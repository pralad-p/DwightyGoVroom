//
// Created by prlpr on 08/09/2023.
//

#ifndef DWIGHTYGOVROOM_STATEHANDLER_HPP
#define DWIGHTYGOVROOM_STATEHANDLER_HPP

#include <mutex>
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/component/component_options.hpp"


class AppState {
private:
    // Private constructor to prevent instantiation
    AppState(): qCounter(0), quitSignal(false) {}

    // Thread-safe instantiation
    static std::once_flag initFlag;
    static AppState* instance;

    // Data members
    int qCounter;
    bool quitSignal;
public:
    bool HandleEvent(const ftxui::Event&, ftxui::ScreenInteractive&, std::shared_ptr<ftxui::ComponentBase>&);
    bool HandleQ(ftxui::ScreenInteractive &);
    void ResetCounters();
    static void quitMethod(ftxui::ScreenInteractive &);
    [[nodiscard]] bool isQuitSignal() const;
    void setQuitSignal(bool quitSignal);

    // Delete copy constructor and assignment operator
    AppState(const AppState&) = delete;
    AppState& operator=(const AppState&) = delete;

    // Function to get the Singleton instance
    static AppState& getInstance();
};

#endif //DWIGHTYGOVROOM_STATEHANDLER_HPP
