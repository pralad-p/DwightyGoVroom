//
// Created by prlpr on 10/09/2023.
//

#include "CommandParser.hpp"
#include "StateHandler.hpp"
#include "ModelEngine.hpp"
#include <regex>

unsigned int parseInputContent(const std::shared_ptr<std::string> &content,
                                           std::vector<std::string>& segments) {
    auto content_string = *content;
    std::regex pattern("#add-goal|#update-goal|#delete-goal");
    if (std::regex_search(content_string, pattern)) {
        // Found the wildcards
        std::smatch match;
        std::regex add_goal_pattern("#add-goal");
        std::regex update_goal_pattern("#update-goal");
        std::regex delete_goal_pattern("#delete-goal");

        if (std::regex_search(content_string,add_goal_pattern)) {
            // Check add-goals configs
            std::regex add_goal_pattern_1("#add-goal ([^\\[]*)");
            std::regex add_goal_pattern_2("#add-goal ([^\\[]*) \\[imp\\] (\\d{2})");
            std::regex add_goal_pattern_3("#add-goal ([^\\[]*) \\[imp\\] (\\d{2}) \\[urg\\] (\\d{2})");
            if (std::regex_search(content_string,match,add_goal_pattern_1)) {
                auto captured = match.str(1);
                if (captured.length() > 0) {
                    segments.at(0) = captured;
                }
            } if (std::regex_search(content_string,match,add_goal_pattern_2)) {
                auto imp_value = match.str(2);
                if (imp_value.length() > 0 && (std::stoi(imp_value) >= 0 && std::stoi(imp_value) <= 10)) {
                    segments.at(1) = imp_value;
                }
            } if (std::regex_search(content_string,match,add_goal_pattern_3)) {
                auto urg_value = match.str(3);
                if (urg_value.length() > 0 && (std::stoi(urg_value) >= 0 && std::stoi(urg_value) <= 10)) {
                    segments.at(2) = urg_value;
                }
                auto &appState = AppState::getInstance();
                if (!appState.isGoodGoalCreation()) {
                    appState.setGoodCreation(true);
                    appState.setSelectedAction(1);
                    Goal g;
                    auto mEngine = ModelEngine::getInstance();
                    g.name = segments.at(0);
                    g.importance = std::stoi(segments.at(1));
                    g.urgency = std::stoi(segments.at(2));
                    g.index = mEngine->getRunningIndex() + 1;
                    mEngine->setRunningIndex(g.index);
                    g.previous_streaks_maintained = 0;
                    g.continuous_days_worked = 0;
                    appState.setTransitGoal(g);
                }
            }
            return 1;
        } else if (std::regex_search(content_string,update_goal_pattern)) {
            // Check update-goals configs
            std::regex update_goal_pattern_1("#update-goal \\[I=([^\\]]*)\\]");
            if (std::regex_search(content_string,match,update_goal_pattern_1)) {
                auto captured = match.str(1);
                std::string inner_captured;
                if (!captured.empty()) {
                    unsigned long long index = std::stoll(captured);
                    auto g = GoalManagerEngine::readGoal(index);
                    if (g) {
                        bool changeFlag = false; // nothing has changed yet
                        std::regex name_param("\\[name=([^\\]]*)\\]");
                        std::regex imp_param("\\[imp=([^\\]]*)\\]");
                        std::regex urg_param("\\[urg=([^\\]]*)\\]");
                        segments.at(0) = g->name;
                        segments.at(1) = std::to_string(g->importance);
                        segments.at(2) = std::to_string(g->urgency);

                        if (std::regex_search(content_string,match,name_param)) {
                            inner_captured = match.str(1);
                            if (!inner_captured.empty()) {
                                segments.at(0) = g->name + " -> " + inner_captured;
                                changeFlag = true;
                            }
                        }
                        if (std::regex_search(content_string,match,imp_param)) {
                            inner_captured = match.str(1);
                            if (!inner_captured.empty() && (std::stoi(inner_captured) >= 0 && std::stoi(inner_captured) <= 10)) {
                                segments.at(1) = std::to_string(g->importance) + " -> " + inner_captured;
                                changeFlag = true;

                            }
                        }
                        if (std::regex_search(content_string,match,urg_param)) {
                            inner_captured = match.str(1);
                            if (!inner_captured.empty() && (std::stoi(inner_captured) >= 0 && std::stoi(inner_captured) <= 10)) {
                                segments.at(2) = std::to_string(g->urgency) + " -> " + inner_captured;
                                changeFlag = true;
                            }
                        }
                        if (changeFlag) {
                            auto &appState = AppState::getInstance();
                                appState.setGoodUpdate(true);
                                appState.setSelectedAction(2);
                                Goal g1;
                                auto mEngine = ModelEngine::getInstance();
                                std::optional<std::pair<std::string, std::string>> result;
                                result = splitByArrow(segments.at(0));
                                if (result) {
                                    auto newResult = std::get<1>(*result);
                                    g1.name = newResult;
                                } else {
                                    g1.name = g->name;
                                }
                                result = splitByArrow(segments.at(1));
                                if (result) {
                                    auto newResult = std::get<1>(*result);
                                    g1.importance = std::stoi(newResult);
                                } else {
                                    g1.importance = g->importance;
                                }
                                result = splitByArrow(segments.at(2));
                                if (result) {
                                    auto newResult = std::get<1>(*result);
                                    g1.urgency = std::stoi(newResult);
                                } else {
                                    g1.urgency = g->urgency;
                                }
                                g1.index = g->index;
                                g1.continuous_days_worked = 0; // when updating goal, reset tracking params
                                g1.previous_streaks_maintained = 0; // when updating goal, reset tracking params
                                appState.setTransitGoal(g1);
                                changeFlag = false;
                        }
                    } else {
                        segments.at(0) = "NOT FOUND";
                    }
                } else {
                    segments.at(0) = "NOT FOUND";
                }
            }
            return 2;

        } else if (std::regex_search(content_string,delete_goal_pattern)) {
            return 3;
        }
    }
    return 0;
}