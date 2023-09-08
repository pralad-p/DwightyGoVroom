//
// Created by prlpr on 03/09/2023.
//

#ifndef DWIGHTYGOVROOM_PRIMUTILITIES_HPP
#define DWIGHTYGOVROOM_PRIMUTILITIES_HPP

// Includes
#include "Goal.hpp"
#include <spdlog/spdlog.h>

// User definitions
#define DGV_GOALS_PATH (std::filesystem::current_path() / "GOALS_DB.json")

#define LOG_CRITICAL(message) do { \
    auto logger = spdlog::get("default_file_logger"); \
    if (logger) { \
        logger->critical(message); \
    } \
} while(0)

#define LOG_INFO(message) do { \
    auto logger = spdlog::get("default_file_logger"); \
    if (logger) { \
        logger->info(message); \
    } \
} while(0)

#define LOG_WARN(message) do { \
    auto logger = spdlog::get("default_file_logger"); \
    if (logger) { \
        logger->warn(message); \
    } \
} while(0)

std::string QuadrantStateEnum2String(QuadrantStateEnum);

#endif //DWIGHTYGOVROOM_PRIMUTILITIES_HPP
