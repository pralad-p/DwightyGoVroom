//
// Created by prlpr on 08/09/2023.
//

#include "ViewEngine.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/component/component_options.hpp"
#include "PrimUtilities.hpp"
#include "WindowsUtilities.hpp"
#include "StateHandler.hpp"
#include "ModelEngine.hpp"

// Initialize static members
std::once_flag ViewEngine::initFlag;
ViewEngine *ViewEngine::instance = nullptr;

ViewEngine &ViewEngine::getInstance() {
    std::call_once(initFlag, []() {
        instance = new ViewEngine();
    });
    return *instance;
}

// Primary method for rendering the UI
void ViewEngine::renderEngine() {
    // Get Model Engine
    ModelEngine *mEngine = ModelEngine::getInstance();
    // Init Screen
    auto screen = ftxui::ScreenInteractive::Fullscreen();

    // Components
    // Time Renderer
    auto timeRenderer = ftxui::Renderer([&] {
        std::string time_str = getCurrentTime();
        return ftxui::text(time_str)
               | color(ftxui::Color::CornflowerBlue)
               | ftxui::bold
               | ftxui::center
               | ftxui::border;
    });

    // Input Component
    auto input_option = ftxui::InputOption();
    auto input_component = ftxui::Input(mEngine->getContentPtr().get(), "Enter text", &input_option);

    auto inputHelpDialogContainer = ftxui::Container::Vertical({
                                                                  ftxui::Renderer([] {
                                                                      return ftxui::text(
                                                                              "Input Help") |
                                                                             ftxui::bold | ftxui::center;
                                                                  }),
                                                                  ftxui::Renderer(
                                                                          [] { return ftxui::separatorHeavy(); }),
                                                                  ftxui::Renderer([&mEngine] {
                                                                      return ftxui::vbox(
                                                                              ftxui::hbox(
                                                                                      ftxui::text("Your content" + *(mEngine->getContentPtr()))
                                                                              ));
                                                                  })
                                                          }) | ftxui::border | ftxui::center;

    auto combinedInputContainer = ftxui::Container::Vertical({
                                                                     inputHelpDialogContainer,
                                                                     input_component,
                                                        });

    // Create a Renderer for the combinedInputContainer.
    auto combinedInputRenderer = ftxui::Renderer(combinedInputContainer, [&] {
        ftxui::Element document;
        auto a = timeRenderer->Focused();

        // Render the inputHelpDialogContainer only when the input_component is in focus.
        if (input_component->Focused()) {
            document = ftxui::vbox({
                                           inputHelpDialogContainer->Render(),
                                           input_component->Render(),
                                   });
        } else {
            document = input_component->Render();
        }

        return document;
    });

    auto applicationContainer = ftxui::Container::Vertical({
                                                                   timeRenderer,
                                                                   //TODO: GoalGrid
                                                                   //TODO: the status bar
                                                                   //TODO: Input box
                                                                   combinedInputRenderer
                                                           });

    // Container for storing the application state (related to events)
    AppState &appState = AppState::getInstance();

    applicationContainer |= ftxui::CatchEvent([&](const ftxui::Event &event) {
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


