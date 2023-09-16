//
// Created by prlpr on 08/09/2023.
//

#include "ViewEngine.hpp"

#include <algorithm>
#include <ftxui-grid-container/grid-container.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>

#include "CommandParser.hpp"
#include "ControllerEngine.hpp"
#include "Hinter.hpp"
#include "ModelEngine.hpp"
#include "PrimUtilities.hpp"
#include "SpecialComponents/DoGoalerComponent.hpp"
#include "StateHandler.hpp"
#include "WindowsUtilities.hpp"

// Initialize static members
std::once_flag ViewEngine::initFlag;
ViewEngine* ViewEngine::instance = nullptr;

ViewEngine& ViewEngine::getInstance() {
    std::call_once(initFlag, []() {
        instance = new ViewEngine();
    });
    return *instance;
}

// HELPERS
ftxui::Element ViewEngine::getRenderElementForInputContainer(const AppState& appState, ftxui::Component& combinedInputContainer,
                                                             ftxui::Component& input_component,
                                                             ftxui::Component& inputHelpDialogContainer) {
    ftxui::Element document;
    // Render the inputHelpDialogContainer only when the input_component is in
    // focus.
    if (combinedInputContainer->Focused()) {
        if (appState.getAdditionalStatusFlag() == ExtraStates::ReadyToLockChanges) {
            document = ftxui::vbox(
                {inputHelpDialogContainer->Render(), input_component->Render() | ftxui::borderStyled(ftxui::Color::Yellow)});
        } else if (appState.getAdditionalStatusFlag() == ExtraStates::LockInModificationChange) {
            document = ftxui::vbox(
                {inputHelpDialogContainer->Render(), input_component->Render() | ftxui::borderStyled(ftxui::Color::Green)});
        } else {
            document = ftxui::vbox({
                inputHelpDialogContainer->Render(),
                input_component->Render() | ftxui::borderRounded,
            });
        }
        combinedInputContainer->ChildAt(1)->TakeFocus();
    } else {
        document = input_component->Render() | ftxui::borderRounded;
    }

    return document;
}

ftxui::Element ViewEngine::getRenderElementForStatusBar(const AppState& appState) {
    switch (appState.getAdditionalStatusFlag()) {
    case ExtraStates::LockOutModificationChange:
        return ftxui::nothing(ftxui::text(""));
    case ExtraStates::LockInModificationChange:
        return ftxui::text("Changes locked in. Hit Enter to confirm.") | color(ftxui::Color::Green) | ftxui::bold;
    case ExtraStates::ReadyToLockChanges:
        return ftxui::text("Hit Alt-V to lock in changes. <Esc> to go back.") | color(ftxui::Color::Red) | ftxui::bold;
    }
}

ftxui::Component ViewEngine::getInputHelpDialogContainer(unsigned int& hintDialogueStatus,
                                                         const std::vector<std::string>& validationSegments) {
    return ftxui::Container::Vertical({ftxui::Renderer([] {
                                           return ftxui::text("Input Help") | ftxui::bold | ftxui::center;
                                       }),
                                       ftxui::Renderer([] {
                                           return ftxui::separatorHeavy();
                                       }),
                                       ftxui::Renderer([&hintDialogueStatus, &validationSegments] {
                                           const unsigned int WINDOW_WIDTH = 80;
                                           return getHintDialogueBasedOnParams(hintDialogueStatus, WINDOW_WIDTH, validationSegments);
                                       })}) |
           ftxui::border | ftxui::center;
}

std::function<void()> confirmActionCallback = []() {
    AppState& appState = AppState::getInstance();
    ModelEngine* mEngine = ModelEngine::getInstance();
    // get action
    auto qItems = appState.getInQueueAction();
    auto action = qItems.first;
    auto goal = qItems.second;

    // do action
    switch (action) {
    case QueuableAction::Add:
        mEngine->getGoalManager().createGoal(goal);
        break;
    case QueuableAction::Update:
        mEngine->getGoalManager().updateGoal(goal);
        break;
    case QueuableAction::Delete:
        mEngine->getGoalManager().deleteGoal(goal.index);
        break;
    case QueuableAction::Achieve:
        break;
    case QueuableAction::None:
        break;
    }
    // update quadrants
    ControllerEngine::updateQuadrants();
    // set original view
    appState.setFocusSelector(0);
};

std::function<void()> denyActionCallback = []() {
    AppState& appState = AppState::getInstance();
    appState.setInQueueAction(QueuableAction::None, {});
    // set original view
    appState.setFocusSelector(0);
};

// Primary method for rendering the UI
void ViewEngine::renderEngine() {

    //  ██ ███    ██ ██ ████████ ██  █████  ██      ██ ███████  █████  ████████ ██  ██████  ███    ██
    //  ██ ████   ██ ██    ██    ██ ██   ██ ██      ██    ███  ██   ██    ██    ██ ██    ██ ████   ██
    //  ██ ██ ██  ██ ██    ██    ██ ███████ ██      ██   ███   ███████    ██    ██ ██    ██ ██ ██  ██
    //  ██ ██  ██ ██ ██    ██    ██ ██   ██ ██      ██  ███    ██   ██    ██    ██ ██    ██ ██  ██ ██
    //  ██ ██   ████ ██    ██    ██ ██   ██ ███████ ██ ███████ ██   ██    ██    ██  ██████  ██   ████

    // Get Model Engine
    ModelEngine* mEngine = ModelEngine::getInstance();
    // Get App Engine
    AppState& appState = AppState::getInstance();
    // Validation segments
    std::vector<std::string> validationSegments{"", "", "", ""};
    // Status of hint Dialog
    unsigned int hintDialogueStatus = 0;
    // Init Screen
    auto screen = ftxui::ScreenInteractive::Fullscreen();

    //   ██████  ██████  ███    ███ ██████   ██████  ███    ██ ███████ ███    ██ ████████ ███████
    //  ██      ██    ██ ████  ████ ██   ██ ██    ██ ████   ██ ██      ████   ██    ██    ██
    //  ██      ██    ██ ██ ████ ██ ██████  ██    ██ ██ ██  ██ █████   ██ ██  ██    ██    ███████
    //  ██      ██    ██ ██  ██  ██ ██      ██    ██ ██  ██ ██ ██      ██  ██ ██    ██         ██
    //   ██████  ██████  ██      ██ ██       ██████  ██   ████ ███████ ██   ████    ██    ███████

    // Time Renderer
    auto timeRenderer = ftxui::Renderer([&] {
        std::string time_str = getCurrentTime();
        return ftxui::text(time_str) | color(ftxui::Color::CornflowerBlue) | ftxui::bold | ftxui::center | ftxui::border;
    });

    // Input Component
    auto input_option = ftxui::InputOption();
    input_option.on_enter = [&validationSegments, &hintDialogueStatus] {
        return AppState::confirmInputActionCallback(validationSegments, hintDialogueStatus);
    };
    input_option.on_change = [&mEngine, &validationSegments, &hintDialogueStatus] {
        hintDialogueStatus = parseInputContent(mEngine->getContentPtr(), validationSegments);
    };
    auto input_component = ftxui::Input(mEngine->getContentPtr().get(), "Enter text", input_option);

    auto inputHelpDialogContainer = getInputHelpDialogContainer(hintDialogueStatus, validationSegments);

    auto combinedInputContainer = ftxui::Container::Vertical({inputHelpDialogContainer, input_component});

    // Create a Renderer for the combinedInputContainer.
    auto combinedInputRenderer =
        ftxui::Renderer(combinedInputContainer, [&appState, &combinedInputContainer, &input_component, &inputHelpDialogContainer] {
            return getRenderElementForInputContainer(appState, combinedInputContainer, input_component, inputHelpDialogContainer);
        });

    // Create a renderer for the Status Bar
    auto statusBar = ftxui::Renderer([&appState] {
        return getRenderElementForStatusBar(appState);
    });

    auto innerDoQuadrant = std::make_shared<ftxui::Component>(ftxui::Container::Vertical({}));
    auto innerScheduleQuadrant = std::make_shared<ftxui::Component>(ftxui::Container::Vertical({}));
    auto innerDelegateQuadrant = std::make_shared<ftxui::Component>(ftxui::Container::Vertical({}));
    auto innerEliminateQuadrant = std::make_shared<ftxui::Component>(ftxui::Container::Vertical({}));

    std::vector<std::shared_ptr<ftxui::Component>> allQuadrants;
    allQuadrants.push_back(innerDoQuadrant);
    allQuadrants.push_back(innerScheduleQuadrant);
    allQuadrants.push_back(innerDelegateQuadrant);
    allQuadrants.push_back(innerEliminateQuadrant);

    ControllerEngine::updateQuadrants(allQuadrants);
    auto doQuadrant = ftxui::Renderer(*innerDoQuadrant, [&] {
        return ftxui::window(ftxui::text("Do"), (*innerDoQuadrant)->Render());
    });
    auto scheduleQuadrant = ftxui::Renderer(*innerScheduleQuadrant, [&] {
        return ftxui::window(ftxui::text("Schedule"), (*innerScheduleQuadrant)->Render());
    });
    auto delegateQuadrant = ftxui::Renderer(*innerDelegateQuadrant, [&] {
        return ftxui::window(ftxui::text("Delegate"), (*innerDelegateQuadrant)->Render());
    });
    auto eliminateQuadrant = ftxui::Renderer(*innerEliminateQuadrant, [&] {
        return ftxui::window(ftxui::text("Delete"), (*innerEliminateQuadrant)->Render());
    });
    auto goalGrid = ftxui::GridContainer({{doQuadrant, scheduleQuadrant}, {delegateQuadrant, eliminateQuadrant}});

    auto applicationContainer = ftxui::Container::Vertical({timeRenderer, goalGrid, combinedInputRenderer, statusBar});

    auto& fs = appState.getFocusSelector();

    auto confirmerBoxContainer = ftxui::Container::Vertical({ftxui::Renderer([] {
                                                                 return ftxui::text("Confirm action?") | ftxui::bold | ftxui::center;
                                                             }),
                                                             ftxui::Renderer([] {
                                                                 return ftxui::separatorHeavy();
                                                             }),
                                                             ftxui::Container::Horizontal({
                                                                 ftxui::Button("✅", confirmActionCallback),
                                                                 ftxui::Button("❌", denyActionCallback),

                                                             })

                                 }) |
                                 ftxui::border | ftxui::center;

    auto completeApplicationContainer = ftxui::Container::Tab({applicationContainer, confirmerBoxContainer}, &fs);

    //  ████████ ██   ██ ██████  ███████  █████  ██████      ██   ██  █████  ███    ██ ██████  ██      ██ ███    ██  ██████
    //     ██    ██   ██ ██   ██ ██      ██   ██ ██   ██     ██   ██ ██   ██ ████   ██ ██   ██ ██      ██ ████   ██ ██
    //     ██    ███████ ██████  █████   ███████ ██   ██     ███████ ███████ ██ ██  ██ ██   ██ ██      ██ ██ ██  ██ ██   ███
    //     ██    ██   ██ ██   ██ ██      ██   ██ ██   ██     ██   ██ ██   ██ ██  ██ ██ ██   ██ ██      ██ ██  ██ ██ ██    ██
    //     ██    ██   ██ ██   ██ ███████ ██   ██ ██████      ██   ██ ██   ██ ██   ████ ██████  ███████ ██ ██   ████  ██████

    // Container for storing the application state (related to events)
    completeApplicationContainer |= ftxui::CatchEvent([&](const ftxui::Event& event) {
        return appState.HandleEvent(event, screen, completeApplicationContainer);
    });

    // Run the application in a loop per second (to render clock without actions)
    std::thread([&] {
        while (!AppState::getInstance().isQuitSignal()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            screen.PostEvent(ftxui::Event::Custom);
        }
    }).detach();

    // Start the event loop
    screen.Clear();
    screen.Loop(completeApplicationContainer);
}
