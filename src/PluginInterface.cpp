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
    response.AddMember("type", REPORT_DAILY_GROUP_TYPE, allocator);
}

extern "C" void DestroyReport() {}

extern "C" void CreateReport(rapidjson::Value& request,
                             rapidjson::Value& response,
                             rapidjson::Document::AllocatorType& allocator,
                             CServerInterface* server) {
    std::string group_mask;
    int from;
    int to;
    int from_two_weeks_ago;
    if (request.HasMember("group") && request["group"].IsString()) {
        group_mask = request["group"].GetString();
    }
    if (request.HasMember("from") && request["from"].IsNumber()) {
        from = request["from"].GetInt();
        from_two_weeks_ago = utils::CalculateTimestampForTwoWeeksAgo(from);
    }
    if (request.HasMember("to") && request["to"].IsNumber()) {
        to = request["to"].GetInt();
    }

    std::vector<ServerLog> log_vector;

    try {
        server->GetLogs(from_two_weeks_ago, to, "", "", &log_vector);
    } catch (const std::exception& e) {
        std::cerr << "[DailyLogsReportInterface]: " << e.what() << std::endl;
    }

    std::cout << "logs size: " << log_vector.size() << std::endl;

    // Лямбда подготавливающая значения double для вставки в AST (округление до 2-х знаков)
    auto format_double_for_AST = [](double value) -> std::string {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << value;
        return oss.str();
    };

    auto create_table = [&](const std::vector<ServerLog>& logs) -> Node {
        std::vector<Node> thead_rows;
        std::vector<Node> tbody_rows;
        std::vector<Node> tfoot_rows;

        // Thead
        thead_rows.push_back(tr({
            th({div({text("Time")})}),
            th({div({text("Type")})}),
            th({div({text("IP")})}),
            th({div({text("Message")})})
        }));

        // Tbody
        for (const auto& log : logs) {
            tbody_rows.push_back(tr({
                td({div({text(log.time)})}),
                td({div({text(log.type)})}),
                td({div({text(log.ip)})}),
                td({div({text(log.message)})})
            }));
        }

        return table({
            thead(thead_rows),
            tbody(tbody_rows),
            tfoot(tfoot_rows),
        }, props({{"className", "table"}}));
    };

    // Total report
    const Node report = div({
        h1({text("Daily Logs Report")}),
        create_table(log_vector)
    });

    utils::CreateUI(report, response, allocator);
}