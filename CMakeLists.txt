cmake_minimum_required(VERSION 3.14)
project(sc)


set(CMAKE_CXX_STANDARD 20)

find_package(spdlog REQUIRED)


include_directories(/usr/local/opt/ncurses/include)

link_directories(/usr/local/opt/ncurses/lib)

set(SOURCE_FILES
        src/logging.h
        src/main.cpp
        src/game.cpp
        src/windows/pause_menu.cpp)

add_executable(sc ${SOURCE_FILES})
target_link_libraries(sc spdlog::spdlog menu ncurses)