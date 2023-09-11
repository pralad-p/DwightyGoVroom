//
// Created by prlpr on 08/09/2023.
//

#ifndef DWIGHTYGOVROOM_VIEWENGINE_HPP
#define DWIGHTYGOVROOM_VIEWENGINE_HPP

#include <mutex>
#include "ftxui/dom/elements.hpp"
#include "StateHandler.hpp"

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

    static ftxui::Element getRendererForInputContainer(const AppState &,
                                                       std::shared_ptr<ftxui::ComponentBase> &,
                                                       const ftxui::Component &);

    static ftxui::Element getRendererForStatusBar(const AppState &);
};

// Special code for HBox simplification
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

#endif //DWIGHTYGOVROOM_VIEWENGINE_HPP
