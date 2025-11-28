cmake_minimum_required(VERSION 3.16)
project(DailyLogsReport)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(DailyLogsReport SHARED
        src/PluginInterface.cpp
        src/Utils.cpp
)

target_include_directories(DailyLogsReport PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/api
        ${CMAKE_SOURCE_DIR}/external
)
