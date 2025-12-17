#include "PluginInterface.hpp"

#include <iomanip>

extern "C" void AboutReport(rapidjson::Value& request,
                            rapidjson::Value& response,
                            rapidjson::Document::AllocatorType& allocator,
                            CServerInterface* server) {
    response.AddMember("version", 1, allocator);
    response.AddMember("name", Value().SetString("Daily Logs report", allocator), allocator);
    response.AddMember("description",
        Value().SetString("Shows platform operation statistics for a specified day, including the number "
            "of regular and error messages in the journal, critical errors, and connection logs.",
             allocator), allocator);
    response.AddMember("type", REPORT_DAILY_TYPE, allocator);
}

extern "C" void DestroyReport() {}

extern "C" void CreateReport(rapidjson::Value& request,
                             rapidjson::Value& response,
                             rapidjson::Document::AllocatorType& allocator,
                             CServerInterface* server) {
    int from;
    int to;
    int from_week_ago;
    if (request.HasMember("from") && request["from"].IsNumber()) {
        from = request["from"].GetInt();
        from_week_ago = utils::CalculateTimestampForWeekAgo(from);
    }
    if (request.HasMember("to") && request["to"].IsNumber()) {
        to = request["to"].GetInt();
    }

    std::vector<ServerLog> all_logs_vector;
    std::vector<ServerLog> requests_logs_vector;
    std::vector<ServerLog> today_logs;
    std::vector<std::string> colors = {"#4A90E2", "#50E3C2", "#F5A623", "#D0021B", "#9013FE"};
    std::string other_color = "#B8E986";

    try {
        server->GetLogs(from_week_ago, to, "", "", &all_logs_vector);
        server->GetLogs(from, to, "REQUEST", "", &requests_logs_vector);
        server->GetLogs(from, to, "", "", &today_logs);
    } catch (const std::exception& e) {
        std::cerr << "[DailyLogsReportInterface]: " << e.what() << std::endl;
    }

    // Server logs chart
    const JSONArray server_logs_chart_data = utils::CreateServerLogsChartData(all_logs_vector);
    const std::vector<std::string> line_keys = {"system", "info", "request", "stop_out", "total"};
    std::vector<Node> line_nodes = {
            // Default nodes
            XAxis({}, props({{"dataKey", "day"}})),
            YAxis(),
            Tooltip(),
            Legend()};

    // Формирование Line nodes
    for (size_t i = 0; i < line_keys.size(); ++i) {
        std::string color = (line_keys[i] == "total") ? other_color : colors[i];

        line_nodes.push_back(
            Line({}, props({
                {"type", "monotone"},
                {"dataKey", line_keys[i]},
                {"stroke", color}
            }))
        );
    }

    Node server_logs_chart_node = ResponsiveContainer({
        LineChart(line_nodes, props({
            {"data", server_logs_chart_data}
        }))
    }, props({
        {"width", "100%"},
        {"height", 300.0}
    }));

    // Top flooder chart
    const JSONArray top_flooders_chart_data = utils::CreateTopFloodersChartData(requests_logs_vector);

    // Вектор Cell с цветами для каждой записи
    std::vector<Node> top_flooders_pie_cells;
    for (size_t i = 0; i < top_flooders_chart_data.size(); ++i) {
        std::string color = i < colors.size() ? colors[i] : other_color;
        top_flooders_pie_cells.push_back(Cell({}, props({{"fill", color}})));
    }

    Node top_flooders_chart = ResponsiveContainer({
        PieChart({
            Tooltip(),
            Legend(),
            Pie(top_flooders_pie_cells, props({
                {"dataKey", "value"},
                {"nameKey", "label"},
                {"data", top_flooders_chart_data},
                {"cx", "50%"},
                {"cy", "50%"},
                {"outerRadius", 100.0},
                {"label", true}
            }))
        })
    }, props({
        {"width", "100%"},
        {"height", 300.0}
    }));

    // Main table
    TableBuilder table_builder("DailyLogsReport");

    table_builder.SetIdColumn("id");
    table_builder.SetOrderBy("id", "DESC");
    table_builder.EnableAutoSave(false);
    table_builder.EnableRefreshButton(false);
    table_builder.EnableBookmarksButton(false);
    table_builder.EnableExportButton(true);

    table_builder.AddColumn({"id", "ID", 1});
    table_builder.AddColumn({"type", "TYPE", 2});
    table_builder.AddColumn({"time", "TIME", 3});
    table_builder.AddColumn({"ip", "IP", 4});
    table_builder.AddColumn({"message", "MESSAGE", 5});

    for (size_t i; i < today_logs.size(); i++) {
        if (today_logs[i].type != "ERROR"
            && today_logs[i].type != "ERROR"
            && today_logs[i].type != "SYSTEM"
            && today_logs[i].type != "DEBUG") {
                table_builder.AddRow({
                    utils::TruncateDouble(static_cast<double>(i), 0),
                    all_logs_vector[i].type,
                    all_logs_vector[i].time,
                    all_logs_vector[i].message
                });
        }
    }

    const JSONObject logs_table_props = table_builder.CreateTableProps();
    const Node logs_table_node = Table({}, logs_table_props);

    // Total report
    const Node report = Column({
        h1({text("Server Logs")}),
        h2({text("Server Messages")}),
        server_logs_chart_node,
        h2({text("Top flooders")}),
        top_flooders_chart,
        h2({text("All Logs")}),
        logs_table_node
    });

    utils::CreateUI(report, response, allocator);
}