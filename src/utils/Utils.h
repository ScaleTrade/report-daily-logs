#pragma once

#include <algorithm>
#include <cctype>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <rapidjson/document.h>
#include <sstream>
#include <string>
#include <vector>

#include "ReportServerInterface.h"
#include "ast/Ast.hpp"
#include "structures/ReportStructures.h"

using namespace ast;

namespace utils {
    void CreateUI(const ast::Node&                    node,
                  rapidjson::Value&                   response,
                  rapidjson::Document::AllocatorType& allocator);

    std::string FormatTimestampToString(const time_t&      timestamp,
                                        const std::string& format = "%Y.%m.%d %H:%M:%S");

    double TruncateDouble(const double& value, const int& digits);

    std::string GetGroupCurrencyByName(const std::vector<ReportGroupRecord>& group_vector,
                                       const std::string&                    group_name);

    int CalculateTimestampForWeekAgo(const int timestamp);

    std::string ExtractDate(const std::string& date);

    bool IsValidIpAddress(const std::string& ip_address);

    JSONArray CreateServerLogsChartData(const std::vector<ReportServerLog>& logs_vector);

    JSONArray CreateTopFloodersChartData(const std::vector<ReportServerLog>& logs_vector);

    std::string NormalizeLogTime(const std::string& time_string);
} // namespace utils