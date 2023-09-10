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
    static ModelEngine* instance;
    static std::mutex instanceMutex;
    static GoalManagerEngine goalManager;
    unsigned long long runningIndex{};
    std::shared_ptr<std::string> content;
    std::mutex contentMutex;

    ModelEngine() : content(std::make_shared<std::string>()) {}; // Private constructor
public:
    ModelEngine(const ModelEngine&) = delete; // Delete copy constructor
    ModelEngine& operator=(const ModelEngine&) = delete; // Delete assignment operator

    // Helpers
    static void initialize(const std::filesystem::path &);
    [[nodiscard]] static const GoalManagerEngine &getGoalManager();
    [[nodiscard]] unsigned long long int getRunningIndex() const;
    void setRunningIndex(unsigned int);
    [[nodiscard]] std::shared_ptr<std::string> getContentPtr();
    static ModelEngine* getInstance();
};

#endif //DWIGHTYGOVROOM_MODELENGINE_HPP
