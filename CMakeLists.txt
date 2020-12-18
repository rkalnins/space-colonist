cmake_minimum_required(VERSION 3.14)
project(sc)


set(CMAKE_CXX_STANDARD 20)

find_package(spdlog REQUIRED)
find_package(Catch2 REQUIRED)

include_directories(/usr/local/opt/ncurses/include)

link_directories(/usr/local/opt/ncurses/lib)

set(TEST_FILES
        src/test/test.cpp)

set(MAIN src/main.cpp)

set(SOURCE_FILES
        src/logging/logging.h
        src/game.cpp
        src/game.h
        src/windows/pause_menu.cpp
        src/windows/pause_menu.h
        src/loop_control/task_pool.cpp
        src/loop_control/task_pool.h
        src/windows/intro.cpp
        src/windows/intro.h
        src/play/objects/spaceship.cpp
        src/play/objects/spaceship.h
        src/play/items/item.cpp
        src/play/items/item.h
        src/play/objects/crew_member.cpp
        src/play/objects/crew_member.h
        src/loop_control/task.cpp
        src/loop_control/task.h
        src/tasks/running_input.h
        src/tasks/pause_input.h
        src/game_tasks.h
        src/input_listener.h
        src/game_state.h
        src/play/spaceship_handler.cpp
        src/play/spaceship_handler.h
        src/play/game_ui.cpp
        src/play/game_ui.h)

add_executable(sc ${MAIN} ${SOURCE_FILES})
add_executable(sc_test ${SOURCE_FILES} ${TEST_FILES})

target_link_libraries(sc spdlog::spdlog menu ncurses)
target_link_libraries(sc_test spdlog::spdlog menu ncurses Catch2::Catch2)