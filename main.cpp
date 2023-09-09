#include <iostream>
#include <spdlog/spdlog.h>
#include "spdlog/sinks/basic_file_sink.h"
#include "ControllerEngine.hpp"

int main() {
    // Set logger
    auto logger = spdlog::basic_logger_mt("default_file_logger", "logs/tui-log.txt", true);

    // Start event loop
    ControllerEngine::getInstance().start();

    // Stop event loop
    ControllerEngine::getInstance().stop();

    // Shutdown logger
    spdlog::shutdown();
    return 0;
}
