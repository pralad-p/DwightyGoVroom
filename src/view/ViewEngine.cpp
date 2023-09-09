//
// Created by prlpr on 08/09/2023.
//

#include <regex>
#include <string>
#include <algorithm>
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

// Template to apply attributes to a text element.
template <typename T, typename... Attributes>
auto ApplyAttributes(T element, Attributes... attributes) {
    return (element | ... | attributes);
}

// Function to create a hbox element with dynamic padding and optional attributes.
template <typename... Attributes>
auto CreateHBox(const std::string& str, int total_width, int left_padding, Attributes... attributes) {
    int right_padding = total_width - static_cast<int>(str.length()) - left_padding;
    if (right_padding < 0) {
        right_padding = 0;
    }
    auto element = ApplyAttributes(ftxui::text(str), attributes...);
    return ftxui::hbox(
            ftxui::text(std::string(left_padding, ' ')),
            element,
            ftxui::text(std::string(right_padding, ' '))
    );
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
                                                                           const unsigned int WINDOW_WIDTH = 60;
                                                                           std::vector<std::string> segments{"","",""};
                                                                           auto status = parseInputContent(
                                                                                   mEngine->getContentPtr());
                                                                           switch (status) {
                                                                               case 0: { // Default message
                                                                                   return ftxui::vbox(
                                                                                           CreateHBox("#add-goal    -> Add a new goal",WINDOW_WIDTH,4),
                                                                                           CreateHBox("#update-goal -> Update an existing goal",WINDOW_WIDTH,4),
                                                                                           CreateHBox("#delete-goal -> Delete an existing goal",WINDOW_WIDTH,4)
                                                                                   );
                                                                               }
                                                                               case 1: { // add-goal related messages
                                                                                   return ftxui::vbox(
                                                                                           CreateHBox("Add a new goal",WINDOW_WIDTH,4,ftxui::underlined),
                                                                                           CreateHBox(" ",WINDOW_WIDTH,4),
                                                                                           CreateHBox("Syntax:",WINDOW_WIDTH,4,ftxui::bold),
                                                                                           CreateHBox("#add-goal <Goal name> [imp] <0-10> [urg] <0-10>",WINDOW_WIDTH,4,ftxui::color(ftxui::Color::Salmon1)),
                                                                                           CreateHBox(" ",WINDOW_WIDTH,4),
                                                                                           CreateHBox("❌  Title: ",WINDOW_WIDTH,6),
                                                                                           CreateHBox("❌  Importance: ",WINDOW_WIDTH,6),
                                                                                           CreateHBox("❌  Urgency: ",WINDOW_WIDTH,6)
                                                                                   );
                                                                               }
                                                                               default: {
                                                                                   return ftxui::Element();
                                                                               }
                                                                           }
                                                                           return ftxui::Element();
                                                                       })
                                                               }) | ftxui::border | ftxui::center;

    auto combinedInputContainer = ftxui::Container::Vertical({
                                                                     inputHelpDialogContainer,
                                                                     input_component
                                                             });

    // Create a Renderer for the combinedInputContainer.
    auto combinedInputRenderer = ftxui::Renderer(combinedInputContainer, [&] {
        ftxui::Element document;
        auto a = timeRenderer->Focused();

        // Render the inputHelpDialogContainer only when the input_component is in focus.
        if (input_component->Focused()) {
            document = ftxui::vbox({
                                           inputHelpDialogContainer->Render(),
                                           input_component->Render() | ftxui::borderRounded,
                                   });
        } else {
            document = input_component->Render() | ftxui::borderRounded;
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

unsigned int ViewEngine::parseInputContent(const std::shared_ptr<std::string> &content) {
    auto content_string = *content;
    std::regex r("#add-goal|#update-goal|#delete-goal");
    if (std::regex_search(content_string, r)) {
        // Found the wildcards
        return 1;
    }
    return 0;
}


