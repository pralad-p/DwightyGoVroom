//
// Created by prlpr on 08/09/2023.
//

#include "StateHandler.hpp"
#include "WindowsUtilities.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/component/component_options.hpp"


// Initialize static members
std::once_flag AppState::initFlag;
AppState *AppState::instance = nullptr;

bool AppState::isQuitSignal() const {
    return quitSignal;
}

void AppState::setQuitSignal(bool signal) {
    AppState::quitSignal = signal;
}

bool AppState::isGoodGoalCreation() const {
    return goodGoalCreation;
}

void AppState::setGoodCreation(bool signal) {
    AppState::goodGoalCreation = signal;
}

int AppState::getSelectedAction() const {
    return selectedAction;
}

void AppState::setSelectedAction(int action) {
    AppState::selectedAction = action;
}

const Goal &AppState::getTransitGoal() const {
    return transitGoal;
}

void AppState::setTransitGoal(const Goal &goal) {
    AppState::transitGoal = goal;
}

bool AppState::isGoodGoalUpdate() const {
    return goodGoalUpdate;
}

void AppState::setGoodUpdate(bool signal) {
    goodGoalUpdate = signal;
}


AppState &AppState::getInstance() {
    std::call_once(initFlag, []() {
        instance = new AppState();
    });
    return *instance;
}


bool AppState::HandleEvent(const ftxui::Event &event, ftxui::ScreenInteractive &screen, std::shared_ptr<ftxui::ComponentBase> &container) {
    if (event == ftxui::Event::Tab) {
        if (!container->ChildAt(0)->ChildAt(1)->Focused()) { // Input box is not focused
            container->ChildAt(0)->ChildAt(1)->TakeFocus();
        } else if (container->ChildAt(0)->ChildAt(1)->Focused()) {
            container->ChildAt(0)->ChildAt(0)->TakeFocus();
            return true;
        }
    }
    else if (event == ftxui::Event::Character('q')) {
        HandleQ(screen);
    } else if (event.is_character()) {
        ResetCounters();
    }
    return false;
}

bool AppState::HandleQ(ftxui::ScreenInteractive &screen) {
    ++qCounter;
    if (qCounter == 3) {
        quitMethod(screen);
        return true;
    }
    return false;
}

void AppState::ResetCounters() {
    qCounter = 0;
}

void AppState::quitMethod(ftxui::ScreenInteractive &screen) {
    screen.ExitLoopClosure()();
    AppState::getInstance().setQuitSignal(true);
    ClearDOSPromptScreen();
}







