//
// Created by prlpr on 03/09/2023.
//

#ifndef DWIGHTYGOVROOM_GOALMANAGERENGINE_HPP
#define DWIGHTYGOVROOM_GOALMANAGERENGINE_HPP

#include "Goal.hpp"
#include <vector>
#include <memory>
#include <optional>

class GoalManagerEngine {
private:
    std::vector<Goal> goals;
public:
    GoalManagerEngine() = default;

    [[nodiscard]] const std::vector<Goal> &getGoals() const;
    void createGoal(Goal&);
    std::optional<Goal> readGoal(const unsigned long long& idx);
    void updateGoal(const Goal& goal);
    void deleteGoal(const unsigned long long& idx);
};

void copyGoalProperties(Goal *destGoal, const Goal &srcGoal);
void setQuadrant(Goal *);



#endif //DWIGHTYGOVROOM_GOALMANAGERENGINE_HPP
