#include "Utils.hpp"

namespace utils {
    void CreateUI(const ast::Node& node,
                  rapidjson::Value& response,
                  rapidjson::Document::AllocatorType& allocator) {
        // Content
        Value node_object(kObjectType);
        to_json(node, node_object, allocator);

        Value content_array(kArrayType);
        content_array.PushBack(node_object, allocator);

        // Header
        Value header_array(kArrayType);

        {
            Value space_object(kObjectType);
            space_object.AddMember("type", "Space", allocator);

            Value children(kArrayType);

            Value text_object(kObjectType);
            text_object.AddMember("type", "#text", allocator);

            Value props(kObjectType);
            props.AddMember("value", "Daily Logs report", allocator);

            text_object.AddMember("props", props, allocator);
            children.PushBack(text_object, allocator);

            space_object.AddMember("children", children, allocator);

            header_array.PushBack(space_object, allocator);
        }

        // Footer
        Value footer_array(kArrayType);

        {
            Value space_object(kObjectType);
            space_object.AddMember("type", "Space", allocator);

            Value props_space(kObjectType);
            props_space.AddMember("justifyContent", "space-between", allocator);
            space_object.AddMember("props", props_space, allocator);

            Value children(kArrayType);

            Value btn_object(kObjectType);
            btn_object.AddMember("type", "Button", allocator);

            Value btn_props_object(kObjectType);
            btn_props_object.AddMember("className", "form_action_button", allocator);
            btn_props_object.AddMember("borderType", "danger", allocator);
            btn_props_object.AddMember("buttonType", "outlined", allocator);

            btn_props_object.AddMember("onClick", "{\"action\":\"CloseModal\"}", allocator);

            btn_object.AddMember("props", btn_props_object, allocator);

            Value btn_children(kArrayType);

            Value text_object(kObjectType);
            text_object.AddMember("type", "#text", allocator);

            Value text_props_object(kObjectType);
            text_props_object.AddMember("value", "Close", allocator);

            text_object.AddMember("props", text_props_object, allocator);
            btn_children.PushBack(text_object, allocator);

            btn_object.AddMember("children", btn_children, allocator);

            children.PushBack(btn_object, allocator);

            space_object.AddMember("children", children, allocator);

            footer_array.PushBack(space_object, allocator);
        }


        // Modal
        Value model_object(kObjectType);
        model_object.AddMember("size", "xxxl", allocator);
        model_object.AddMember("headerContent", header_array, allocator);
        model_object.AddMember("footerContent", footer_array, allocator);
        model_object.AddMember("content", content_array, allocator);


        // UI
        Value ui_object(kObjectType);
        ui_object.AddMember("modal", model_object, allocator);

        response.SetObject();
        response.AddMember("ui", ui_object, allocator);
    }

    int CalculateTimestampForTwoWeeksAgo(const int timestamp) {
        constexpr int two_weeks_interval = 14 * 24 * 60 * 60;
        return timestamp - two_weeks_interval;
    }

    std::string ExtractDate(const std::string& date_string) {
        if (date_string.size() > 10) {
            return date_string.substr(0, 10);
        }
        return date_string;
    }

    JSONArray CreateServerLogsChartData(const std::vector<ServerLog>& log_vector) {
        std::map<std::string, LogCountPoint> logs_map;

        for (const auto& log : log_vector) {
            std::string day = ExtractDate(log.time);

            auto& point = logs_map[day];
            point.date = day;

            if (log.type == "SYSTEM") {
                point.system++;
            }
            else if (log.type == "INFO") {
                point.info++;
            }
            else if (log.type == "REQUEST") {
                point.request++;
            }
            else if (log.type == "STOP_OUT") {
                point.stop_out++;
            }

            point.total++;
        }

        std::vector<LogCountPoint> data_points;
        data_points.reserve(logs_map.size());

        for (const auto& [date, point] : logs_map) {
            data_points.push_back(point);
        }

        std::sort(data_points.begin(), data_points.end(),
            [](const LogCountPoint& a, const LogCountPoint& b) {
                return a.date < b.date;
            });

        JSONArray chart_data;
        for (const auto& point : data_points) {
            JSONObject row;
            row["day"]      = JSONValue(point.date);
            row["system"]   = JSONValue(static_cast<double>(point.system));
            row["info"]     = JSONValue(static_cast<double>(point.info));
            row["request"]  = JSONValue(static_cast<double>(point.request));
            row["stop_out"] = JSONValue(static_cast<double>(point.stop_out));
            row["total"]    = JSONValue(static_cast<double>(point.total));

            chart_data.emplace_back(row);
        }

        return chart_data;
    }
}