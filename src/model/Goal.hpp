//
// Created by prlpr on 03/09/2023.
//

#ifndef DWIGHTYGOVROOM_GOAL_HPP
#define DWIGHTYGOVROOM_GOAL_HPP

#include <string>

enum class QuadrantStateEnum {
    DO,
    SCHEDULE,
    DELEGATE,
    DELETE
};

struct Goal {
    unsigned long long index;
    std::string name;
    std::string lastDateLogged;
    bool isMultiDay;
    unsigned int importance;
    unsigned int urgency;
    unsigned int continuous_days_worked;
    unsigned int previous_streaks_maintained;
    QuadrantStateEnum current_quadrant;
};


#endif //DWIGHTYGOVROOM_GOAL_HPP
