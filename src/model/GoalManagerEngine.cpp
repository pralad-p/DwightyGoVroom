//
// Created by prlpr on 03/09/2023.
//

#include "GoalManagerEngine.hpp"

void GoalManagerEngine::createGoal(Goal &goal) {
    setQuadrant(&goal);
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
            copyGoalProperties(&goalItr, goal);
        }
    }
}

void GoalManagerEngine::deleteGoal(const unsigned long long int &idx) {
    auto newIteratorEnd = std::remove_if(this->goals.begin(), this->goals.end(), [idx](const Goal& g) {
        return g.index == idx;
    });
    this->goals.erase(newIteratorEnd, this->goals.end());
}

const std::vector<Goal> &GoalManagerEngine::getGoals() const {
    return goals;
}


void copyGoalProperties(Goal *destGoal, const Goal &srcGoal) {
    destGoal->name = (destGoal->name != srcGoal.name) ? srcGoal.name : destGoal->name;
    destGoal->importance = (destGoal->importance != srcGoal.importance) ? srcGoal.importance : destGoal->importance;
    destGoal->urgency = (destGoal->urgency != srcGoal.urgency) ? srcGoal.urgency : destGoal->urgency;
    setQuadrant(destGoal);
}


void setQuadrant(Goal *g) {
    if ((g->importance >= 5) && (g->urgency >= 5)) {
        // This has to be done immediately
        g->current_quadrant = QuadrantStateEnum::DO;
    } else if ((g->importance >= 5) && (g->urgency <= 4)) {
        // This is not that urgent, but is still important
        g->current_quadrant = QuadrantStateEnum::SCHEDULE;
    } else if ((g->urgency >= 5) && (g->importance <= 4)) {
        // This can be delegated off
        g->current_quadrant = QuadrantStateEnum::DELEGATE;
    } else {
        // This must be of delete preference to the user
        g->current_quadrant = QuadrantStateEnum::DELETE;
    }
}
