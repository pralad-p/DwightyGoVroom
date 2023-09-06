//
// Created by prlpr on 03/09/2023.
//

#ifndef DWIGHTYGOVROOM_JSONMANAGERENGINE_HPP
#define DWIGHTYGOVROOM_JSONMANAGERENGINE_HPP

#include <filesystem>
#include <ModelEngine.hpp>
#include "PrimUtilities.hpp"

class JSONManager {
public:
    static void readFromJSON();
    static void writeToJSON();
};

#endif //DWIGHTYGOVROOM_JSONMANAGERENGINE_HPP
