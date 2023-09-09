//
// Created by prlpr on 03/09/2023.
//

#include "PrimUtilities.hpp"

std::string QuadrantStateEnum2String(QuadrantStateEnum state) {
    switch (state) {
        case QuadrantStateEnum::DO:
            return "DO";
        case QuadrantStateEnum::SCHEDULE:
            return "SCHEDULE";
        case QuadrantStateEnum::DELEGATE:
            return "DELEGATE";
        case QuadrantStateEnum::DELETE:
            return "DELETE";
        default:
            return "UNKNOWN";
    }
}

std::string getCurrentTime() {
    std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
    time_t t = std::chrono::system_clock::to_time_t(p);
    char str[26];
    ctime_s(str, sizeof str, &t);
    return str;
}
