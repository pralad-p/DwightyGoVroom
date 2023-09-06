//
// Created by prlpr on 03/09/2023.
//

#include "GoalManagerEngine.hpp"

void GoalManagerEngine::createGoal(Goal &goal) {
    if (!GoalManagerEngine::validateGoal(goal)) { // new goal fails validation
        LOG_CRITICAL("Goal object has issues during creation.");
        throw std::runtime_error("Goal object has issues during creation.");
    }
    setQuadrant(&goal);
    GoalManagerEngine::goals.push_back(goal);
}

std::optional<Goal> GoalManagerEngine::readGoal(const unsigned long long int &idx) {
    for (const auto &goal: GoalManagerEngine::goals) {
        if (goal.index == idx) {
            return goal;
        }
    }
    auto logger = spdlog::get("default_file_logger");
    logger->critical("Could not find index to read from");
    return std::nullopt; // Couldn't find index, exceptional case
}

void GoalManagerEngine::updateGoal(const Goal &goal) {
    for (auto &goalItr: GoalManagerEngine::goals) {
        if (goalItr.index == goal.index) {
            // Found collective goal to update
            copyGoalProperties(&goalItr, goal);
            if (!GoalManagerEngine::validateGoal(goalItr)) {
                throw std::runtime_error("Goal object has issues during updating.");
            }
            break;
        }
    }
}

void GoalManagerEngine::deleteGoal(const unsigned long long int &idx) {
    auto newIteratorEnd = std::remove_if(GoalManagerEngine::goals.begin(), GoalManagerEngine::goals.end()
                                         , [idx](const Goal& g) {
        return g.index == idx;
    });
    GoalManagerEngine::goals.erase(newIteratorEnd, GoalManagerEngine::goals.end());
}

const std::vector<Goal> &GoalManagerEngine::getGoals() {
    return goals;
}

bool GoalManagerEngine::validateGoal(const Goal &g) {
    auto getMaxIndex = [](const std::vector<Goal>& scopedGoals) -> unsigned long long {
        unsigned long long maxIdx = 0;
        for (const auto &goal : scopedGoals) {
            if (goal.index > maxIdx) {
                maxIdx = goal.index;
            }
        }
        return maxIdx;
    };
    auto maxIdx = getMaxIndex(goals);
    if (g.index <= maxIdx) {
        return false;
    }
    if (g.name.empty()) {
        return false;
    }
    if (g.importance > 10 || g.urgency > 10) {
        return false;
    }
    return true;
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
