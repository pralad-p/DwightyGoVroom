//
// Created by prlpr on 03/09/2023.
//

#include "ControllerEngine.hpp"
#include "JsonManagerEngine.hpp"
#include "ViewEngine.hpp"

void ControllerEngine::start() {

    // Init. the JsonManagerEngine
    JSONManager::readFromJSON();

    // ViewEngine::Render
    ViewEngine::getInstance().renderEngine();
}

void ControllerEngine::stop() {
    // Any cleanup code here
//    JSONManager::writeToJSON();
}

// Function to get Singleton instance
ControllerEngine &ControllerEngine::getInstance() {
    static ControllerEngine instance;  // Thread-safe
    return instance;
}