cmake_minimum_required(VERSION 3.16)
project(BlackjackGUI CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# raylib is downloaded and built automatically the first time.
# Nothing to install by hand - it only needs internet on the first configure.
# ---------------------------------------------------------------------------
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(raylib)

# Card and Hand live in the repo root and are shared with the console
# version - they are NOT duplicated here.
#
# Deck.cpp is the one exception: this folder has its OWN copy (see
# gui/Deck.cpp) with a one-line fix to how the shuffle is seeded. The
# console version's Deck.cpp in the repo root is untouched.
add_executable(BlackjackGUI
    main.cpp
    Deck.cpp
    ../Card.cpp
    ../Hand.cpp
)

target_include_directories(BlackjackGUI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(BlackjackGUI PRIVATE raylib)

# On Windows: clean window, no black console behind it.
if(MSVC)
    set_target_properties(BlackjackGUI PROPERTIES
        WIN32_EXECUTABLE TRUE
        LINK_FLAGS "/ENTRY:mainCRTStartup"
    )
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        PROPERTY VS_STARTUP_PROJECT BlackjackGUI)
endif()
