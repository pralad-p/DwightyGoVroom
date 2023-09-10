//
// Created by prlpr on 03/09/2023.
//

#include "PrimUtilities.hpp"
#include <string>
#include <optional>
#include <utility>
#include <string_view>
#include <sstream>

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

// Get date as string, format is YYYY-MM-DD
std::string getcurrentDateAsString() {
    std::time_t t = std::time(nullptr);   // get time now
    std::tm* now = std::localtime(&t);
    std::stringstream date;
    date << std::to_string(now->tm_year + 1900);
    date << "-";
    date << std::to_string((now->tm_mon + 1));
    date << "-";
    date << std::to_string(now->tm_mday);
    return date.str();
}


std::string_view trimLeadingSpaces(std::string_view str) {
    size_t start = 0;
    while (start < str.size() && std::isspace(str[start])) {
        ++start;
    }
    return str.substr(start);
}


std::optional<std::pair<std::string, std::string>> splitByArrow(const std::string &input) {
    auto pos = input.find("->");
    if (pos == std::string::npos) {
        return std::nullopt;
    }
    std::string_view firstHalf = trimLeadingSpaces(input.substr(0, pos));
    std::string_view secondHalf = trimLeadingSpaces(input.substr(pos + 2));
    return std::make_pair(std::string(firstHalf), std::string(secondHalf));
}
