#pragma once

struct LogCountPoint {
    std::string date;
    int system = 0;
    int info = 0;
    int request = 0;
    int stop_out = 0;
    int total = 0;
};