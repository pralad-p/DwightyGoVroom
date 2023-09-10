//
// Created by prlpr on 08/09/2023.
//

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
#include "CommandParser.hpp"

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
    // Get App Engine
    AppState &appState = AppState::getInstance();
    // Validation segments
    std::vector<std::string> validationSegments{"","","",""};
    // Status of hint Dialog
    unsigned int hintDialogueStatus = 0;

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
    input_option.on_enter = [&validationSegments, &hintDialogueStatus] {
        return AppState::confirmActionCallback(validationSegments, hintDialogueStatus);
    };
    input_option.on_change = [&mEngine, &validationSegments, &hintDialogueStatus] {
        hintDialogueStatus = parseInputContent(mEngine->getContentPtr(), validationSegments);
    };
    auto input_component = ftxui::Input(mEngine->getContentPtr().get(), "Enter text", &input_option);

    auto inputHelpDialogContainer = ftxui::Container::Vertical({
                                                                       ftxui::Renderer([] {
                                                                           return ftxui::text(
                                                                                   "Input Help") |
                                                                                  ftxui::bold | ftxui::center;
                                                                       }),
                                                                       ftxui::Renderer(
                                                                               [] { return ftxui::separatorHeavy(); }),
                                                                       ftxui::Renderer([&mEngine, &hintDialogueStatus, &validationSegments] {
                                                                           const unsigned int WINDOW_WIDTH = 80;
                                                                           switch (hintDialogueStatus) {
                                                                               case 0: { // Default message
                                                                                   return ftxui::vbox(
                                                                                           CreateHBox("#add-goal    -> Add a new goal",WINDOW_WIDTH,4),
                                                                                           CreateHBox("#update-goal -> Update an existing goal",WINDOW_WIDTH,4),
                                                                                           CreateHBox("#delete-goal -> Delete an existing goal",WINDOW_WIDTH,4)
                                                                                   );
                                                                               }
                                                                               case 1: { // add-goal related messages
                                                                                   std::string titleString = (validationSegments.at(0).length() > 0) ? ("✅  Title: " + validationSegments.at(0)) : "❌  Title: ";
                                                                                   std::string importanceString = (validationSegments.at(1).length() > 0) ? ("✅  Importance: " + validationSegments.at(1)) : "❌  Importance: ";
                                                                                   std::string urgencyString = (validationSegments.at(2).length() > 0) ? ("✅  Urgency: " + validationSegments.at(2)) : "❌  Urgency: ";
                                                                                   std::string isMultiDayString = (validationSegments.at(3) == "y" || validationSegments.at(3) == "n") ? ("✅  Multi-day task? " + validationSegments.at(3)) : "❌  Multi-day task? ";
                                                                                   return ftxui::vbox(
                                                                                           CreateHBox("Add a new goal",WINDOW_WIDTH,4,ftxui::underlined),
                                                                                           CreateHBox(" ",WINDOW_WIDTH,4),
                                                                                           CreateHBox("Syntax:",WINDOW_WIDTH,4,ftxui::bold),
                                                                                           CreateHBox("#add-goal <Goal name> [imp] <00-10> [urg] <00-10> [md] <y|n>",WINDOW_WIDTH,4,ftxui::color(ftxui::Color::Salmon1)),
                                                                                           CreateHBox(" ",WINDOW_WIDTH,4),
                                                                                           CreateHBox(titleString,WINDOW_WIDTH,6),
                                                                                           CreateHBox(importanceString,WINDOW_WIDTH,6),
                                                                                           CreateHBox(urgencyString,WINDOW_WIDTH,6),
                                                                                           CreateHBox(isMultiDayString,WINDOW_WIDTH,6)
                                                                                   );
                                                                               }
                                                                               case 2: {
                                                                                   ftxui::Elements portions;
                                                                                   portions.push_back(CreateHBox("Update an existing goal",WINDOW_WIDTH,4,ftxui::underlined));
                                                                                   portions.push_back(CreateHBox(" ",WINDOW_WIDTH,4));
                                                                                   portions.push_back(CreateHBox("Syntax:",WINDOW_WIDTH,4,ftxui::bold));
                                                                                   portions.push_back(CreateHBox("#update-goal [I=<index>] [name=<New Name>] [imp=<00-10>] [urg=<00-10>] [md=<y|n>]",WINDOW_WIDTH,4,ftxui::color(ftxui::Color::Salmon1)));
                                                                                   portions.push_back(CreateHBox(" ",WINDOW_WIDTH,4));
                                                                                   if (validationSegments.at(0) == "NOT FOUND") {
                                                                                       portions.push_back(CreateHBox("No goal with that index. Try again.",WINDOW_WIDTH,4, ftxui::color(ftxui::Color::Red)));
                                                                                   }  else {
                                                                                       std::string titleString = " Current title: " + validationSegments.at(0);
                                                                                       std::string importanceString = " Current importance: " + validationSegments.at(1);
                                                                                       std::string urgencyString = " Current urgency: " + validationSegments.at(2);
                                                                                       std::string isMultiDayString = " Is Multi-day? " + validationSegments.at(3);
                                                                                       portions.push_back(CreateHBox(titleString,WINDOW_WIDTH,6));
                                                                                       portions.push_back(CreateHBox(importanceString,WINDOW_WIDTH,6));
                                                                                       portions.push_back(CreateHBox(urgencyString,WINDOW_WIDTH,6));
                                                                                       portions.push_back(CreateHBox(isMultiDayString,WINDOW_WIDTH,6));
                                                                                   }
                                                                                   return ftxui::vbox(portions);
                                                                               }
                                                                               case 3: {
                                                                                   ftxui::Elements portions;
                                                                                   portions.push_back(CreateHBox("Delete an existing goal",WINDOW_WIDTH,4,ftxui::underlined));
                                                                                   portions.push_back(CreateHBox(" ",WINDOW_WIDTH,4));
                                                                                   portions.push_back(CreateHBox("Syntax:",WINDOW_WIDTH,4,ftxui::bold));
                                                                                   portions.push_back(CreateHBox("#delete-goal [I=<index>]",WINDOW_WIDTH,4,ftxui::color(ftxui::Color::Salmon1)));
                                                                                   portions.push_back(CreateHBox(" ",WINDOW_WIDTH,4));
                                                                                   if (validationSegments.at(0) == "NOT FOUND") {
                                                                                       portions.push_back(CreateHBox("No goal with that index. Try again.",WINDOW_WIDTH,4, ftxui::color(ftxui::Color::Red)));
                                                                                   }  else {
                                                                                       std::string titleString = " Current title: " + validationSegments.at(0);
                                                                                       std::string importanceString = " Current importance: " + validationSegments.at(1);
                                                                                       std::string urgencyString = " Current urgency: " + validationSegments.at(2);
                                                                                       std::string isMultiDayString = " Is Multi-day? " + validationSegments.at(3);
                                                                                       portions.push_back(CreateHBox(titleString,WINDOW_WIDTH,6));
                                                                                       portions.push_back(CreateHBox(importanceString,WINDOW_WIDTH,6));
                                                                                       portions.push_back(CreateHBox(urgencyString,WINDOW_WIDTH,6));
                                                                                       portions.push_back(CreateHBox(isMultiDayString,WINDOW_WIDTH,6));
                                                                                   }
                                                                                   return ftxui::vbox(portions);
                                                                               }
                                                                               default: {
                                                                                   return ftxui::nothing(ftxui::text(""));
                                                                               }
                                                                           }
                                                                       })
                                                               }) | ftxui::border | ftxui::center;

    auto combinedInputContainer = ftxui::Container::Vertical({
                                                                     inputHelpDialogContainer,
                                                                     input_component
                                                             });

    // Create a Renderer for the combinedInputContainer.
    auto combinedInputRenderer = ftxui::Renderer(combinedInputContainer, [&] {
        ftxui::Element document;
        // Render the inputHelpDialogContainer only when the input_component is in focus.
        if (input_component->Focused()) {
            if (appState.getAdditionalStatusFlag() == ExtraStates::ReadyToLockChanges) {
                document = ftxui::vbox({
                                               inputHelpDialogContainer->Render(),
                                               input_component->Render() | ftxui::borderStyled(ftxui::Color::Yellow)
                                       });
            } else if (appState.getAdditionalStatusFlag() == ExtraStates::LockInModificationChange) {
                document = ftxui::vbox({
                                               inputHelpDialogContainer->Render(),
                                               input_component->Render() | ftxui::borderStyled(ftxui::Color::Green)
                                       });
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
    });

    // Create a renderer for the Status Bar
    auto statusBar = ftxui::Renderer([&appState] {
                if (appState.getAdditionalStatusFlag() == ExtraStates::LockInModificationChange) {
                    return ftxui::text("Changes locked in. Hit Enter to confirm.") | color(ftxui::Color::Green) |
                           ftxui::bold;
                } else if (appState.getAdditionalStatusFlag() == ExtraStates::ReadyToLockChanges) {
                    return ftxui::text("Hit Alt-V to lock in changes.") | color(ftxui::Color::Red) |
                           ftxui::bold;
                } else {
                    return ftxui::nothing(ftxui::text(""));
                }
            });

    auto applicationContainer = ftxui::Container::Vertical({
                                                                   timeRenderer,
                                                                   //GoalGrid
                                                                   combinedInputRenderer,
                                                                   statusBar
                                                           });

    // Container for storing the application state (related to events)

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

