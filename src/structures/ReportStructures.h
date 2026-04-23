#pragma once

struct LogCountPoint {
    std::string date;
    int         client  = 0;
    int         manager = 0;
    int         system  = 0;
    int         total   = 0;
};