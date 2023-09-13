cmake_minimum_required(VERSION 3.16)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v4.1.1
)
FetchContent_Declare(ftxui-grid-container
        GIT_REPOSITORY https://github.com/mingsheng13/grid-container-ftxui
        GIT_TAG master
)

FetchContent_GetProperties(ftxui)
if (NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()
FetchContent_MakeAvailable(ftxui-grid-container)
# ------------------------------------------------------------------------------


# --- Fetch JSON --------------------------------------------------------------
FetchContent_Declare(json
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)
# ------------------------------------------------------------------------------

# --- Fetch SPDLOG: Logging library ---------------------------------------------
FetchContent_Declare(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.12.0)
FetchContent_MakeAvailable(spdlog)
# ------------------------------------------------------------------------------

#if (MSVC) # Do not know if this is still needed. Doesn't work with address sanitizer.
#    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#endif ()

project(DwightyGoVroom)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        main.cpp
        src/model/ModelEngine.cpp
        src/controller/ControllerEngine.cpp
        src/controller/StateHandler.cpp
        src/controller/CommandParser.cpp
        src/model/GoalManagerEngine.cpp
        src/model/ModelEngine.cpp
        src/model/JsonManagerEngine.cpp
        src/utils/PrimUtilities.cpp
        src/view/ViewEngine.cpp
        src/view/Hinter.cpp
        src/view/SpecialComponents/DoGoalerComponent.cpp
)

set(HEADERS
        src/model/ModelEngine.hpp
        src/model/Goal.hpp
        src/model/GoalManagerEngine.hpp
        src/model/ModelEngine.hpp
        src/model/JsonManagerEngine.hpp
        src/utils/PrimUtilities.hpp
        src/utils/WindowsUtilities.hpp
        src/controller/ControllerEngine.hpp
        src/controller/StateHandler.hpp
        src/controller/CommandParser.hpp
        src/view/ViewEngine.hpp
        src/view/Hinter.hpp
        src/view/SpecialComponents/DoGoalerComponent.hpp
)

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address") # Address Sanitizer (works with WSL toolchain)

add_executable(DwightyGoVroom)

target_sources(DwightyGoVroom PRIVATE ${SOURCES} ${HEADERS})

target_include_directories(DwightyGoVroom PRIVATE src/controller src/model src/view src/utils)

target_link_libraries(DwightyGoVroom
        PRIVATE ftxui-grid-container
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
        PRIVATE nlohmann_json::nlohmann_json
        PRIVATE spdlog::spdlog
)
