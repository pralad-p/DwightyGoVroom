//
// Created by prlpr on 13/09/2023.
//

#include "DoGoalerComponent.hpp"
#include "StateHandler.hpp"

#include <utility>
ftxui::Element DoGoalerComponent::Render() {
    return main_container_->Render();
}
DoGoalerComponent::DoGoalerComponent(Goal g, std::string taskText, std::string impLevel, std::string urgLevel)
    : goal_(std::move(g))
    , taskText_(std::move(taskText))
    , impLevel_(std::move(impLevel))
    , urgLevel_(std::move(urgLevel))
    , completeActionMethod_([this]() {
        this->completeAction();
    })
    , deleteActionMethod_([this]() {
        this->deleteAction();
    }) {
    if (goal_.isMultiDay) {
        button_container_->Add(workedOnButton_);
    }
    button_container_->Add(completeButton_);
    button_container_->Add(deleteButton_);
    auto staticRenderer = ftxui::Renderer([&] {
        return ftxui::hbox(ftxui::text(taskText_), ftxui::text(impLevel_) | ftxui::color(ftxui::Color::Red), ftxui::text(urgLevel_));
    });
    main_container_->Add(staticRenderer);
    main_container_->Add(button_container_);
    Add(main_container_);
}

void DoGoalerComponent::completeAction() {
    AppState& appState = AppState::getInstance();
    appState.setInQueueAction(QueuableAction::Delete, goal_);
    appState.setFocusSelector(1);
}


void DoGoalerComponent::deleteAction() {
    AppState& appState = AppState::getInstance();
    appState.setInQueueAction(QueuableAction::Delete, goal_);
    appState.setFocusSelector(1);
}
