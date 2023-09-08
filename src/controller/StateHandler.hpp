//
// Created by prlpr on 08/09/2023.
//

#ifndef DWIGHTYGOVROOM_STATEHANDLER_HPP
#define DWIGHTYGOVROOM_STATEHANDLER_HPP

#include <mutex>

class AppState {
private:
    // Private constructor to prevent instantiation
    AppState(): quitSignal(false) {}
    // Thread-safe instantiation
    static std::once_flag initFlag;
    static AppState* instance;
    // Data members
    bool quitSignal;
public:
    [[nodiscard]] bool isQuitSignal() const;
    void setQuitSignal(bool quitSignal);

    // Delete copy constructor and assignment operator
    AppState(const AppState&) = delete;
    AppState& operator=(const AppState&) = delete;

    // Function to get the Singleton instance
    static AppState& getInstance() {
        std::call_once(initFlag, [](){
            instance = new AppState();
        });
        return *instance;
    }
};

// Initialize static members
std::once_flag AppState::initFlag;
AppState* AppState::instance = nullptr;

bool AppState::isQuitSignal() const {
    return quitSignal;
}

void AppState::setQuitSignal(bool signal) {
    AppState::quitSignal = signal;
}


#endif //DWIGHTYGOVROOM_STATEHANDLER_HPP
