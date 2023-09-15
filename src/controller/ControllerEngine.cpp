//
// Created by prlpr on 03/09/2023.
//

#include "ControllerEngine.hpp"
#include "JsonManagerEngine.hpp"
#include "SpecialComponents/DoGoalerComponent.hpp"
#include "ViewEngine.hpp"
#include <sstream>

void ControllerEngine::start() {

    // Init. the JsonManagerEngine
    JSONManager::readFromJSON();

    // ViewEngine::Render
    ViewEngine::getInstance().renderEngine();
}

void ControllerEngine::stop() {
    // Any cleanup code here
    JSONManager::writeToJSON();
}

// Function to get Singleton instance
ControllerEngine &ControllerEngine::getInstance() {
    static ControllerEngine instance;  // Thread-safe
    return instance;
}

void ControllerEngine::updateQuadrants(std::vector<std::shared_ptr<ftxui::Component>>& quadrants) {
    static std::vector<std::shared_ptr<ftxui::Component>> storedQuadrants = quadrants;

    // If a new vector of quadrants is provided, update the storedQuadrants
    if (!quadrants.empty()) {
        storedQuadrants = quadrants;
    }

    auto mEngine = ModelEngine::getInstance();
    auto goals = mEngine->getGoalManager().getGoals();


    for (const auto& g: goals) {
        if (g.current_quadrant == QuadrantStateEnum::DO) {
            auto &quadrant = *storedQuadrants.at(0);
            //(*innerDoQuadrant)->Add(doContainer1 | ftxui::border);
            std::stringstream taskText;
            std::string impLevel, urgLevel;
            taskText << std::to_string(g.index);
            taskText << ". ";
            taskText << g.name;
            if (g.importance <= 10 && g.importance >= 8) {
                impLevel = " ‼‼‼";
            } else if (g.importance <= 7 && g.importance >= 6) {
                impLevel = " ‼‼";
            } else {
                impLevel = " ‼";
            }
            if (g.urgency <= 10 && g.urgency >= 8) {
                urgLevel = " 🎯🎯🎯";
            } else if (g.urgency <= 7 && g.urgency >= 6) {
                urgLevel = " 🎯🎯";
            } else {
                urgLevel = " 🎯";
            }
            quadrant->Add(std::make_shared<DoGoalerComponent>(taskText.str(),impLevel, urgLevel) | ftxui::border);
        } else if (g.current_quadrant == QuadrantStateEnum::SCHEDULE) {

        } else if (g.current_quadrant == QuadrantStateEnum::DELEGATE) {

        } else { // DELETE

        }
    }




}

void ControllerEngine::updateQuadrants() {
    std::vector<std::shared_ptr<ftxui::Component>> defaultQuadrants;
    updateQuadrants(defaultQuadrants);
}
