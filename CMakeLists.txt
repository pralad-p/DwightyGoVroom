cmake_minimum_required(VERSION 3.16)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v4.1.1
)

FetchContent_GetProperties(ftxui)
if (NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()

# ------------------------------------------------------------------------------

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

project(DwightyGoVroom)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        main.cpp

)

set(HEADERS

)

add_executable(DwightyGoVroom)

target_sources(DwightyGoVroom PRIVATE ${SOURCES} ${HEADERS})

target_include_directories(DwightyGoVroom PRIVATE src)

target_link_libraries(DwightyGoVroom
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
)
