//
// Created by prlpr on 03/09/2023.
//

#ifndef DWIGHTYGOVROOM_GOALMANAGERENGINE_HPP
#define DWIGHTYGOVROOM_GOALMANAGERENGINE_HPP

#include <vector>
#include <memory>
#include <optional>
#include <stdexcept>
#include "Goal.hpp"
#include "PrimUtilities.hpp"

class GoalManagerEngine {
private:
    static std::vector<Goal> goals;
    static std::mutex goals_mutex;
    static void copyGoalProperties(Goal *destGoal, const Goal &srcGoal);
    static void setQuadrant(Goal *);
public:
    GoalManagerEngine() = default;

    [[nodiscard]] static const std::vector<Goal> &getGoals() ;
    static void createGoal(Goal&);
    static std::optional<Goal> readGoal(const unsigned long long&);
    static void updateGoal(const Goal&);
    static void deleteGoal(const unsigned long long&);
    static bool validateGoal(const Goal &);
};


#endif //DWIGHTYGOVROOM_GOALMANAGERENGINE_HPP
