//
// Created by prlpr on 02/09/2023.
//

#include <sstream>
#include "ModelEngine.hpp"

ModelEngine* ModelEngine::instance = nullptr;
std::mutex ModelEngine::instanceMutex;
GoalManagerEngine ModelEngine::goalManager;
std::mutex initializeMutex; // Mutex for the initialize method

ModelEngine *ModelEngine::getInstance() {
    if (instance == nullptr) {
        std::lock_guard<std::mutex> lock(instanceMutex);
        if (instance == nullptr)  // Double-checked locking
        {
            instance = new ModelEngine();
        }
    }
    return instance;
}

void ModelEngine::initialize(const std::filesystem::path &JSONPath) {
    if (!std::filesystem::exists((JSONPath))) {
        // First run
        LOG_INFO("First run, hence there is no JSON file loaded.");
        return;
    }

    std::lock_guard<std::mutex> lock(initializeMutex);
    std::ifstream file(JSONPath);
    if (!file.is_open()) {
        LOG_CRITICAL("Failed to open the JSON file");
        exit(1);
    }
    nlohmann::json JsonConfigBlock;
    try {
        file >> JsonConfigBlock;
    } catch (nlohmann::json::parse_error &e) {
        std::stringstream logMessage;
        logMessage << "JSON Parse error occurred: ";
        logMessage << e.what();
        LOG_CRITICAL(logMessage.str());
        exit(1);
    }
    auto mEngine = ModelEngine::getInstance(); // Thread-safe getInstance()
    if (JsonConfigBlock.contains("runningIndex")) {

        mEngine->runningIndex = JsonConfigBlock["runningIndex"];
    }
    for (const auto &goalIter: JsonConfigBlock["goalManager"]["goals"]) {
        Goal g;
        g.index = goalIter["index"];
        g.name = goalIter["name"];
        g.importance = goalIter["importance"];
        g.urgency = goalIter["urgency"];
        g.continuous_days_worked = goalIter["continuous_days_worked"];
        g.previous_streaks_maintained = goalIter["previous_streaks_maintained"];
        auto stateString = goalIter["current_state"];
        if (stateString == "DO") {
            g.current_quadrant = QuadrantStateEnum::DO;
        } else if (stateString == "SCHEDULE") {
            g.current_quadrant = QuadrantStateEnum::SCHEDULE;
        } else if (stateString == "DELEGATE") {
            g.current_quadrant = QuadrantStateEnum::DELEGATE;
        } else { // DELETE case
            g.current_quadrant = QuadrantStateEnum::DELETE;
        }
        GoalManagerEngine::createGoal(g);
    }
    file.close();
}

unsigned long long int ModelEngine::getRunningIndex() const {
    return runningIndex;
}

const GoalManagerEngine &ModelEngine::getGoalManager() {
    return goalManager;
}


