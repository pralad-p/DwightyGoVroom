//
// Created by prlpr on 03/09/2023.
//

#ifndef DWIGHTYGOVROOM_PRIMUTILITIES_HPP
#define DWIGHTYGOVROOM_PRIMUTILITIES_HPP

#define DGV_GOALS_PATH (std::filesystem::current_path() / "GOALS_DB.json")

#include "Goal.hpp"

std::string QuadrantStateEnum2String(QuadrantStateEnum);

#endif //DWIGHTYGOVROOM_PRIMUTILITIES_HPP
