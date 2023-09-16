//
// Created by prlpr on 13/09/2023.
//

#ifndef DWIGHTYGOVROOM_DOGOALERCOMPONENT_HPP
#define DWIGHTYGOVROOM_DOGOALERCOMPONENT_HPP

#include "Goal.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <utility>

class DoGoalerComponent : public ftxui::ComponentBase {
public:
    DoGoalerComponent(Goal g,std::string taskText, std::string impLevel, std::string urgLevel);

    ftxui::Element Render() override;

    std::function<void()> completeActionMethod_;
    std::function<void()> deleteActionMethod_;

    void completeAction();
    void deleteAction();
private:
    Goal goal_;
    long long index_{};
    std::string taskText_;
    std::string impLevel_;
    std::string urgLevel_;
    ftxui::Component workedOnButton_ = ftxui::Button("⏫", nullptr, ftxui::ButtonOption::Animated());
    ftxui::Component completeButton_ = ftxui::Button("✅", completeActionMethod_, ftxui::ButtonOption::Animated());
    ftxui::Component deleteButton_ = ftxui::Button("❌", deleteActionMethod_, ftxui::ButtonOption::Animated());
    ftxui::Component button_container_ = ftxui::Container::Horizontal({});
    ftxui::Component main_container_ = ftxui::Container::Vertical({});
};

#endif // DWIGHTYGOVROOM_DOGOALERCOMPONENT_HPP
