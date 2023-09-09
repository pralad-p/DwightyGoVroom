//
// Created by prlpr on 08/09/2023.
//

#ifndef DWIGHTYGOVROOM_VIEWENGINE_HPP
#define DWIGHTYGOVROOM_VIEWENGINE_HPP

#include <mutex>

class ViewEngine {
private:
    // Private constructor to prevent instantiation
    ViewEngine() = default;
    // Thread-safe instantiation
    static std::once_flag initFlag;
    static ViewEngine* instance;
public:
    // Delete copy constructor and assignment operator
    ViewEngine(const ViewEngine&) = delete;
    ViewEngine& operator=(const ViewEngine&) = delete;

    // Function to get the Singleton instance
    static ViewEngine& getInstance();
    static void renderEngine();

    static unsigned int parseInputContent(const std::shared_ptr<std::string>& content);
};

#endif //DWIGHTYGOVROOM_VIEWENGINE_HPP
