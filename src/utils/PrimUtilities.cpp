//
// Created by prlpr on 03/09/2023.
//

#include "PrimUtilities.hpp"
#include <string>
#include <optional>
#include <utility>
#include <string_view>

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
