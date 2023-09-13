//
// Created by prlpr on 13/09/2023.
//

#include "DoGoalerComponent.hpp"
ftxui::Element
DoGoalerComponent::Render()
{
  return main_container_->Render();
}
DoGoalerComponent::DoGoalerComponent(std::string taskText,
                                     std::string impLevel,
                                     std::string urgLevel)
  : taskText_(std::move(taskText))
  , impLevel_(std::move(impLevel))
  , urgLevel_(std::move(urgLevel))
{
  button_container_->Add(workedOnButton_);
  button_container_->Add(completeButton_);
  button_container_->Add(deleteButton_);
  auto staticRenderer = ftxui::Renderer([&] {
    return ftxui::hbox(ftxui::text(taskText_),
                       ftxui::text(impLevel_) | ftxui::color(ftxui::Color::Red),
                       ftxui::text(urgLevel_));
  });
  main_container_->Add(staticRenderer);
  main_container_->Add(button_container_);
  Add(main_container_);
}
