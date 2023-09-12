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
#include "Hinter.hpp"
#include "ModelEngine.hpp"
#include "PrimUtilities.hpp"
#include "StateHandler.hpp"
#include "WindowsUtilities.hpp"

// Initialize static members
std::once_flag ViewEngine::initFlag;
ViewEngine* ViewEngine::instance = nullptr;

ViewEngine& ViewEngine::getInstance() {
  std::call_once(initFlag, []() { instance = new ViewEngine(); });
  return *instance;
}

// HELPERS
ftxui::Element ViewEngine::getRendererForInputContainer(
    const AppState& appState,
    ftxui::Component& input_component,
    const ftxui::Component& inputHelpDialogContainer) {
  ftxui::Element document;
  // Render the inputHelpDialogContainer only when the input_component is in
  // focus.
  if (input_component->Focused()) {
    if (appState.getAdditionalStatusFlag() == ExtraStates::ReadyToLockChanges) {
      document = ftxui::vbox({inputHelpDialogContainer->Render(),
                              input_component->Render() |
                                  ftxui::borderStyled(ftxui::Color::Yellow)});
    } else if (appState.getAdditionalStatusFlag() ==
               ExtraStates::LockInModificationChange) {
      document = ftxui::vbox({inputHelpDialogContainer->Render(),
                              input_component->Render() |
                                  ftxui::borderStyled(ftxui::Color::Green)});
    } else {
      document = ftxui::vbox({
          inputHelpDialogContainer->Render(),
          input_component->Render() | ftxui::borderRounded,
      });
    }
  } else {
    document = input_component->Render() | ftxui::borderRounded;
  }

  return document;
}

ftxui::Element ViewEngine::getRendererForStatusBar(const AppState& appState) {
  if (appState.getAdditionalStatusFlag() ==
      ExtraStates::LockInModificationChange) {
    return ftxui::text("Changes locked in. Hit Enter to confirm.") |
           color(ftxui::Color::Green) | ftxui::bold;
  } else if (appState.getAdditionalStatusFlag() ==
             ExtraStates::ReadyToLockChanges) {
    return ftxui::text("Hit Alt-V to lock in changes.") |
           color(ftxui::Color::Red) | ftxui::bold;
  } else {
    return ftxui::nothing(ftxui::text(""));
  }
}

// Primary method for rendering the UI
void ViewEngine::renderEngine() {
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

  // Components
  // Time Renderer
  auto timeRenderer = ftxui::Renderer([&] {
    std::string time_str = getCurrentTime();
    return ftxui::text(time_str) | color(ftxui::Color::CornflowerBlue) |
           ftxui::bold | ftxui::center | ftxui::border;
  });

  // Input Component
  auto input_option = ftxui::InputOption();
  input_option.on_enter = [&validationSegments, &hintDialogueStatus] {
    return AppState::confirmActionCallback(validationSegments,
                                           hintDialogueStatus);
  };
  input_option.on_change = [&mEngine, &validationSegments,
                            &hintDialogueStatus] {
    hintDialogueStatus =
        parseInputContent(mEngine->getContentPtr(), validationSegments);
  };
  auto input_component =
      ftxui::Input(mEngine->getContentPtr().get(), "Enter text", input_option);

  auto inputHelpDialogContainer =
      ftxui::Container::Vertical(
          {ftxui::Renderer([] {
             return ftxui::text("Input Help") | ftxui::bold | ftxui::center;
           }),
           ftxui::Renderer([] { return ftxui::separatorHeavy(); }),
           ftxui::Renderer([&hintDialogueStatus, &validationSegments] {
             const unsigned int WINDOW_WIDTH = 80;
             return getHintDialogueBasedOnParams(
                 hintDialogueStatus, WINDOW_WIDTH, validationSegments);
           })}) |
      ftxui::border | ftxui::center;

  auto combinedInputContainer =
      ftxui::Container::Vertical({inputHelpDialogContainer, input_component});

  // Create a Renderer for the combinedInputContainer.
  auto combinedInputRenderer = ftxui::Renderer(
      combinedInputContainer,
      [&appState, &input_component, &inputHelpDialogContainer] {
        return getRendererForInputContainer(appState, input_component,
                                            inputHelpDialogContainer);
      });

  // Create a renderer for the Status Bar
  auto statusBar = ftxui::Renderer(
      [&appState] { return getRendererForStatusBar(appState); });

  auto applicationContainer =
      ftxui::Container::Vertical({timeRenderer,
                                  // GoalGrid
                                  combinedInputRenderer, statusBar});

  // Container for storing the application state (related to events)

  applicationContainer |= ftxui::CatchEvent([&](const ftxui::Event& event) {
    return appState.HandleEvent(event, screen, applicationContainer);
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
  screen.Loop(applicationContainer);
}
