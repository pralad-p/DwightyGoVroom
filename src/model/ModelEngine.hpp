//
// Created by prlpr on 02/09/2023.
//

#ifndef DWIGHTYGOVROOM_MODELENGINE_HPP
#define DWIGHTYGOVROOM_MODELENGINE_HPP

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include "GoalManagerEngine.hpp"

class ModelEngine {
private:
    static std::shared_ptr<ModelEngine> instance;
    static GoalManagerEngine goalManager;
    unsigned long long runningIndex{};
public:
    // Constructors
    ModelEngine() = default;
    explicit ModelEngine(const std::filesystem::path &);

    // Helpers
    [[nodiscard]] static const GoalManagerEngine &getGoalManager();
    [[nodiscard]] unsigned long long int getRunningIndex() const;
    static std::shared_ptr<ModelEngine> getInstance();
};

#endif //DWIGHTYGOVROOM_MODELENGINE_HPP
