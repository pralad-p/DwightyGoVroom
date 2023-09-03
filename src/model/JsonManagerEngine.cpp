//
// Created by prlpr on 03/09/2023.
//

#include "JsonManagerEngine.hpp"
#include "PrimUtilities.hpp"

ModelEngine JSONManager::readFromJSON() {
    // TODO: Check if GOALS_DB.json present in Application home dir
    std::filesystem::path goalsDbPath =  DGV_GOALS_PATH;

    if (std::filesystem::exists(goalsDbPath)) {
        // TODO: If not present, return empty construction (first run)
        return {};
    }
    // TODO: If present, parse and return constructed Model Engine
    return ModelEngine(goalsDbPath);
}

void JSONManager::writeToJSON() {
    nlohmann::json JSONObject;
    // Serialize runningIndex
    JSONObject["runningIndex"] = ModelEngine::getInstance()->getRunningIndex();
    // Serialize goals
    nlohmann::json goalArray = nlohmann::json::array();
    for (const auto &goal: ModelEngine::getInstance()->getGoalManager().getGoals()) {
        nlohmann::json goalJson;
        goalJson["index"] = goal.index;
        goalJson["name"] = goal.name;
        goalJson["importance"] = goal.importance;
        goalJson["urgency"] = goal.urgency;
        goalJson["continuous_days_worked"] = goal.continuous_days_worked;
        goalJson["continuous_days_worked"] = goal.previous_streaks_maintained;
        goalJson["current_quadrant"] = QuadrantStateEnum2String(goal.current_quadrant);
        goalArray.push_back(goalJson);
    }
    JSONObject["goals"] = goalArray;
    auto DbPath = DGV_GOALS_PATH;
    std::ofstream outputFile(DbPath);
    if (outputFile.is_open()) {
        outputFile << JSONObject.dump(4);  // Pretty print with 4-space indent
        outputFile.close();
    } else {
        throw std::runtime_error("Failed to open " + DbPath.string() + " for writing.");
    }
}
