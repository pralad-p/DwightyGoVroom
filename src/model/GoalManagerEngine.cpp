//
// Created by prlpr on 03/09/2023.
//

#include "GoalManagerEngine.hpp"

// Initialize static members
std::vector<Goal> GoalManagerEngine::goals;
std::mutex GoalManagerEngine::goals_mutex;

void GoalManagerEngine::createGoal(Goal &goal) {
    std::lock_guard<std::mutex> lock(goals_mutex);
    if (!GoalManagerEngine::validateGoal(goal)) { // new goal fails validation
        LOG_CRITICAL("Goal object has issues during creation.");
        throw std::runtime_error("Goal object has issues during creation.");
    }
    setQuadrant(&goal);
    GoalManagerEngine::goals.push_back(goal);
}

std::optional<Goal> GoalManagerEngine::readGoal(const unsigned long long int &idx) {
    std::lock_guard<std::mutex> lock(goals_mutex);
    for (const auto &goal: GoalManagerEngine::goals) {
        if (goal.index == idx) {
            return goal;
        }
    }
    LOG_WARN("Could not find goal associated with index");
    return std::nullopt; // Couldn't find index, exceptional case
}

void GoalManagerEngine::updateGoal(const Goal &goal) {
    std::lock_guard<std::mutex> lock(goals_mutex);
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
    std::lock_guard<std::mutex> lock(goals_mutex);
    auto newIteratorEnd = std::remove_if(GoalManagerEngine::goals.begin(), GoalManagerEngine::goals.end(),
                                         [idx](const Goal &g) {
                                             return g.index == idx;
                                         });
    GoalManagerEngine::goals.erase(newIteratorEnd, GoalManagerEngine::goals.end());
}

const std::vector<Goal> &GoalManagerEngine::getGoals() {
    std::lock_guard<std::mutex> lock(goals_mutex);
    return goals;
}

bool GoalManagerEngine::validateGoal(const Goal &g) {
    auto getMaxIndex = [](const std::vector<Goal> &scopedGoals) -> unsigned long long {
        unsigned long long maxIdx = 0;
        for (const auto &goal: scopedGoals) {
            if (goal.index > maxIdx) {
                maxIdx = goal.index;
            }
        }
        return maxIdx;
    };
    auto maxIdx = getMaxIndex(goals);
    if (g.index < maxIdx) {
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


void GoalManagerEngine::copyGoalProperties(Goal *destGoal, const Goal &srcGoal) {
    destGoal->name = srcGoal.name;
    destGoal->importance = srcGoal.importance;
    destGoal->urgency = srcGoal.urgency;
    setQuadrant(destGoal);
}


void GoalManagerEngine::setQuadrant(Goal *g) {
    if (g->importance >= 5) {
        g->current_quadrant = (g->urgency >= 5)
                ? QuadrantStateEnum::DO
                : QuadrantStateEnum::SCHEDULE;
    } else {
        g->current_quadrant = (g->urgency >= 5)
                ? QuadrantStateEnum::DELEGATE
                : QuadrantStateEnum::DELETE;
    }
}
