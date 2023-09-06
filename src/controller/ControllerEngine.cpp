//
// Created by prlpr on 03/09/2023.
//

#include "ControllerEngine.hpp"
#include "CommandParser.hpp"
#include "ActionExecutor.hpp"
#include "ModelEngine.hpp"

void ControllerEngine::start() {
    // Initialize CommandParser and ActionExecutor
    CommandParser parser;
    ActionExecutor executor;

    // Your main application loop goes here
    // 1. Wait for user input
    // 2. Parse it using CommandParser
    // 3. Execute the action using ActionExecutor
}

void ControllerEngine::stop() {
    // Any cleanup code here
}

// Function to get Singleton instance
ControllerEngine& ControllerEngine::getInstance() {
    static ControllerEngine instance;  // Thread-safe
    return instance;
}