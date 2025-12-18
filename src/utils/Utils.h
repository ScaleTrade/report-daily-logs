#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <cctype>
#include "ast/Ast.hpp"
#include <rapidjson/document.h>
#include "Structures.h"
#include "structures/PluginStructures.h"

using namespace ast;

namespace utils {
    void CreateUI(const ast::Node& node,
              rapidjson::Value& response,
              rapidjson::Document::AllocatorType& allocator);

    std::string FormatTimestampToString(const time_t& timestamp);

    double TruncateDouble(const double& value, const int& digits);

    std::string GetGroupCurrencyByName(const std::vector<GroupRecord>& group_vector, const std::string& group_name);

    int CalculateTimestampForWeekAgo(const int timestamp);

    std::string ExtractDate(const std::string& date);

    bool IsValidIpAddress(const std::string& ip_address);

    JSONArray CreateServerLogsChartData(const std::vector<ServerLog>& logs_vector);

    JSONArray CreateTopFloodersChartData(const std::vector<ServerLog>& logs_vector);

    std::string NormalizeLogTime(const std::string& time_string);
}