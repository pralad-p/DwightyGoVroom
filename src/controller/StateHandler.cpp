//
// Created by prlpr on 08/09/2023.
//

#include "StateHandler.hpp"
#include "WindowsUtilities.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component_options.hpp>
#include "GoalManagerEngine.hpp"
#include "ModelEngine.hpp"


// Initialize static members
std::once_flag AppState::initFlag;
AppState *AppState::instance = nullptr;

bool AppState::isQuitSignal() const {
    return quitSignal;
}

void AppState::setQuitSignal(bool signal) {
    AppState::quitSignal = signal;
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

AppState &AppState::getInstance() {
    std::call_once(initFlag, []() {
        instance = new AppState();
    });
    return *instance;
}


bool AppState::HandleEvent(const ftxui::Event &event, ftxui::ScreenInteractive &screen, std::shared_ptr<ftxui::ComponentBase> &container) {
    if (event == ftxui::Event::Tab) {
        if (!container->ChildAt(0)->ChildAt(2)->Focused()) { // Input box is not focused
            container->ChildAt(0)->ChildAt(2)->TakeFocus();
            return true;
        } else if (container->ChildAt(0)->ChildAt(2)->Focused()) {
            container->ChildAt(0)->ChildAt(0)->TakeFocus();
            return true;
        }
    }
    else if (event == ftxui::Event::Special("\x1Bv")) { // ASCII value for Alt+V
        bool inputBoxFocused = container->ChildAt(0)->ChildAt(2)->Focused();
        if (inputBoxFocused) {
            if (getAdditionalStatusFlag() == ExtraStates::ReadyToLockChanges) {
                if ((selectedAction == 1) || (selectedAction == 2) || (selectedAction == 3)) {
                    setAdditionalStatusFlag(ExtraStates::LockInModificationChange);
                    return true;
                }
            }
            return false;
        }
    } else if (event == ftxui::Event::Escape) {
        if (getAdditionalStatusFlag() == ExtraStates::LockInModificationChange) {
            setAdditionalStatusFlag(ExtraStates::ReadyToLockChanges);
            return true;
        }
        return false;
    } else if (event == ftxui::Event::Character('q')) {
        HandleQ(screen);
    } else if (event.is_character()) {
        ResetCounters();
    }
    return false;
}


void AppState::confirmActionCallback(std::vector<std::string> &segments, unsigned int &hintStatus) {
    auto &appState = AppState::getInstance();
    if (appState.getAdditionalStatusFlag() == ExtraStates::LockInModificationChange) {
        if (appState.getSelectedAction() == 1) {
            // 1 => Time to add new goal
            auto mEngine = ModelEngine::getInstance();
            Goal g = appState.getTransitGoal();
            // set index here (to prevent re-additions during handwritten mistakes)
            g.index = mEngine->getRunningIndex() + 1;
            mEngine->setRunningIndex(g.index);
            GoalManagerEngine::createGoal(g);
            LOG_INFO("Goal created (" + g.name + ") INDEX: [" + std::to_string(g.index) + "]");
            mEngine->getContentPtr()->clear();
            appState.setTransitGoal(Goal {});
            appState.setAdditionalStatusFlag(ExtraStates::LockOutModificationChange);
            // Hint dialogue related
            for (auto& s: segments) {
                s.clear();
            }
            hintStatus = 0;
        } else if (appState.getSelectedAction() == 2) {
            // 2 => Time to update existing goal
            Goal g = appState.getTransitGoal();
            GoalManagerEngine::updateGoal(g);
            LOG_INFO("Goal updated (" + g.name + ") INDEX: [" + std::to_string(g.index) + "]");
            auto mEngine = ModelEngine::getInstance();
            mEngine->getContentPtr()->clear();
            appState.setTransitGoal(Goal {});
            appState.setAdditionalStatusFlag(ExtraStates::LockOutModificationChange);
            // Hint dialogue related
            for (auto& s: segments) {
                s.clear();
            }
            hintStatus = 0;
        } else if (appState.getSelectedAction() == 3) {
            // 3 => Time to delete existing goal
            // Good goal to delete
            Goal g = appState.getTransitGoal();
            GoalManagerEngine::deleteGoal(g.index);
            LOG_INFO("Goal deleted -> INDEX: [" + std::to_string(g.index) + "]");
            auto mEngine = ModelEngine::getInstance();
            mEngine->getContentPtr()->clear();
            appState.setTransitGoal(Goal {});
            appState.setAdditionalStatusFlag(ExtraStates::LockOutModificationChange);
            // Hint dialogue related
            for (auto& s: segments) {
                s.clear();
            }
            hintStatus = 0;
        }
        appState.setSelectedAction(-1);
    }
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

ExtraStates AppState::getAdditionalStatusFlag() const {
    return additionalStatusFlag;
}

void AppState::setAdditionalStatusFlag(ExtraStates flag) {
    AppState::additionalStatusFlag = flag;
}






