//
// Created by prlpr on 03/09/2023.
//

#ifndef DWIGHTYGOVROOM_CONTROLLERENGINE_HPP
#define DWIGHTYGOVROOM_CONTROLLERENGINE_HPP

#include <vector>
#include <memory>
#include <ftxui/component/component.hpp>

class ControllerEngine {
private:
    ControllerEngine() = default;

public:
    static ControllerEngine &getInstance();

    static void start();

    static void stop();

    static void updateQuadrants(std::vector<std::shared_ptr<ftxui::Component>>&);

    static void updateQuadrants();

    // Delete copy and assignment operators for singleton safety
    ControllerEngine(const ControllerEngine &) = delete;

    void operator=(const ControllerEngine &) = delete;
};


#endif //DWIGHTYGOVROOM_CONTROLLERENGINE_HPP
