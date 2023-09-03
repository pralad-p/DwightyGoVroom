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
    void createGoal(const Goal& goal);
    std::optional<Goal> readGoal(const unsigned long long& idx);
    void updateGoal(const Goal& goal);
    void deleteGoal(const unsigned long long& idx);
    GoalManagerEngine() = default;
};

void updateProperty(Goal *, const Goal &);



#endif //DWIGHTYGOVROOM_GOALMANAGERENGINE_HPP
