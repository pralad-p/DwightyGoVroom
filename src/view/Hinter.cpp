//
// Created by prlpr on 11/09/2023.
//

#include "Hinter.hpp"
#include "ViewEngine.hpp"


ftxui::Element getHintDialogueBasedOnParams(const unsigned int &hintDialogueStatus, const int &WINDOW_WIDTH,
                                            const std::vector<std::string> &validationSegments) {
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
}

