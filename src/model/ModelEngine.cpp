//
// Created by prlpr on 02/09/2023.
//

#include "ModelEngine.hpp"

std::shared_ptr<ModelEngine> ModelEngine::getInstance() {
    if (instance == nullptr) {
        instance = std::make_shared<ModelEngine>();
    }
    return instance;
}

ModelEngine::ModelEngine(const std::filesystem::path &JSONPath) {
    std::ifstream file(JSONPath);
    nlohmann::json JsonConfigBlock;
    file >> JsonConfigBlock;
    if (JsonConfigBlock.contains("runningIndex")) {
        this->runningIndex = JsonConfigBlock["runningIndex"];
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
        this->goalManager.createGoal(g);
    }
    file.close();
}

unsigned long long int ModelEngine::getRunningIndex() const {
    return runningIndex;
}

const GoalManagerEngine &ModelEngine::getGoalManager() const {
    return goalManager;
}

