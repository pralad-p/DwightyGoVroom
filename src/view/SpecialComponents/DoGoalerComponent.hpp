//
// Created by prlpr on 13/09/2023.
//

#ifndef DWIGHTYGOVROOM_DOGOALERCOMPONENT_HPP
#define DWIGHTYGOVROOM_DOGOALERCOMPONENT_HPP

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <utility>

class DoGoalerComponent : public ftxui::ComponentBase
{
public:
  DoGoalerComponent(std::string taskText,
                    std::string impLevel,
                    std::string urgLevel);

  ftxui::Element Render() override;

private:
  std::string taskText_;
  std::string impLevel_;
  std::string urgLevel_;
  ftxui::Component workedOnButton_ =
    ftxui::Button("Achieved?", nullptr, ftxui::ButtonOption::Simple());
  ftxui::Component completeButton_ =
    ftxui::Button("Completed?", nullptr, ftxui::ButtonOption::Simple());
  ftxui::Component deleteButton_ =
    ftxui::Button("Not worth it?", nullptr, ftxui::ButtonOption::Simple());
  ftxui::Component button_container_ = ftxui::Container::Horizontal({});
  ftxui::Component main_container_ = ftxui::Container::Vertical({});
};

#endif // DWIGHTYGOVROOM_DOGOALERCOMPONENT_HPP
