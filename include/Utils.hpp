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
#include "ast/Ast.hpp"
#include <rapidjson/document.h>
#include "Structures.hpp"
#include "PluginStructures.hpp"

using namespace ast;

namespace utils {
    void CreateUI(const ast::Node& node,
              rapidjson::Value& response,
              rapidjson::Document::AllocatorType& allocator);

    int CalculateTimestampForTwoWeeksAgo(const int timestamp);

    std::string ExtractDate(const std::string& date);

    JSONArray CreateServerLogsChartData(const std::vector<ServerLog>& logs_vector);

    JSONArray CreateTopFloodersChartData(const std::vector<ServerLog>& logs_vector);
}