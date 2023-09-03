//
// Created by prlpr on 03/09/2023.
//

#include "GoalManagerEngine.hpp"


void GoalManagerEngine::createGoal(const Goal &goal) {
    this->goals.push_back(goal);
}

std::optional<Goal> GoalManagerEngine::readGoal(const unsigned long long int &idx) {
    for (const auto &goal: this->goals) {
        if (goal.index == idx) {
            return goal;
        }
    }
    return std::nullopt; // Couldn't find index, exceptional case
}

void GoalManagerEngine::updateGoal(const Goal &goal) {
    for (auto &goalItr: this->goals) {
        if (goalItr.index == goal.index) {
            // Found collective goal to update
            updateProperty(&goalItr, goal);
        }
    }
}

void GoalManagerEngine::deleteGoal(const unsigned long long int &idx) {
    auto newIteratorEnd = std::remove_if(this->goals.begin(), this->goals.end(), [idx](const Goal& g) {
        return g.index == idx;
    });
    this->goals.erase(newIteratorEnd, this->goals.end());
}


void updateProperty(Goal *destGoal, const Goal &srcGoal) {
    destGoal->name = (destGoal->name != srcGoal.name) ? srcGoal.name : destGoal->name;
    destGoal->importance = (destGoal->importance != srcGoal.importance) ? srcGoal.importance : destGoal->importance;
    destGoal->urgency = (destGoal->urgency != srcGoal.urgency) ? srcGoal.urgency : destGoal->urgency;
}

