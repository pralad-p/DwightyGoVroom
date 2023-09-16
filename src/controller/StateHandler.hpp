//
// Created by prlpr on 08/09/2023.
//

#ifndef DWIGHTYGOVROOM_STATEHANDLER_HPP
#define DWIGHTYGOVROOM_STATEHANDLER_HPP

#include "Goal.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include <mutex>

enum class ExtraStates {
    LockOutModificationChange,
    LockInModificationChange,
    ReadyToLockChanges,
};

enum class QueuableAction { Add, Update, Delete, Achieve, None };

class AppState {
private:
    // Private constructor to prevent instantiation
    AppState()
        : qCounter(0)
        , quitSignal(false)
        , selectedAction(-1)
        , additionalStatusFlag(ExtraStates::LockOutModificationChange)
        , focusSelector(0)
        , qAction(QueuableAction::None) {}

    // Thread-safe instantiation
    static std::once_flag initFlag;
    static AppState* instance;

    // Data members
    int qCounter;
    int focusSelector;
    std::atomic<bool> quitSignal;
    int selectedAction;
    ExtraStates additionalStatusFlag;
    QueuableAction qAction;
    Goal transitGoal, queueGoal;

public:
    bool HandleEvent(const ftxui::Event&, ftxui::ScreenInteractive&, std::shared_ptr<ftxui::ComponentBase>&);
    bool HandleQ(ftxui::ScreenInteractive&);
    void ResetCounters();
    static void quitMethod(ftxui::ScreenInteractive&);
    std::pair<QueuableAction, Goal> getInQueueAction();
    void setInQueueAction(QueuableAction, Goal);
    int& getFocusSelector();
    void setFocusSelector(int);
    [[nodiscard]] bool isQuitSignal() const;
    void setQuitSignal(bool);
    [[nodiscard]] int getSelectedAction() const;
    void setSelectedAction(int);
    [[nodiscard]] ExtraStates getAdditionalStatusFlag() const;
    void setAdditionalStatusFlag(ExtraStates);
    [[nodiscard]] const Goal& getTransitGoal() const;
    void setTransitGoal(const Goal&);
    static void confirmInputActionCallback(std::vector<std::string>&, unsigned int&);

    // Delete copy constructor and assignment operator
    AppState(const AppState&) = delete;
    AppState& operator=(const AppState&) = delete;

    // Function to get the Singleton instance
    static AppState& getInstance();
};

#endif // DWIGHTYGOVROOM_STATEHANDLER_HPP
