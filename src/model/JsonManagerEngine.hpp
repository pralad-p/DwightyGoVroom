//
// Created by prlpr on 03/09/2023.
//

#ifndef DWIGHTYGOVROOM_JSONMANAGERENGINE_HPP
#define DWIGHTYGOVROOM_JSONMANAGERENGINE_HPP

#include <ModelEngine.hpp>
#include <filesystem>

class JSONManager {
public:
    static ModelEngine readFromJSON();
    static void writeToJSON();
};

#endif //DWIGHTYGOVROOM_JSONMANAGERENGINE_HPP
