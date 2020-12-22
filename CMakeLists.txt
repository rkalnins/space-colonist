cmake_minimum_required(VERSION 3.14)
project(sc)


set(CMAKE_CXX_STANDARD 20)

find_package(spdlog REQUIRED)
find_package(Catch2 REQUIRED)
find_package(effolkronium_random REQUIRED)

include_directories(/usr/local/opt/ncurses/include)

link_directories(/usr/local/opt/ncurses/lib)

set(TEST_FILES
        src/test/test.cpp)

set(MAIN src/main.cpp)

set(SOURCE_FILES
        src/loop_control/watchdog.h
        src/logging/logging.h
        src/game.cpp
        src/game.h
        src/loop_control/task_pool.cpp
        src/loop_control/task_pool.h
        src/windows/intro.cpp
        src/windows/intro.h
        src/play/spaceship.cpp
        src/play/spaceship.h
        src/play/item.cpp
        src/play/item.h
        src/play/crew_member.cpp
        src/play/crew_member.h
        src/loop_control/task.cpp
        src/loop_control/task.h
        src/game_deps.h
        src/input_listener.h
        src/game_state.h
        src/play/spaceship_handler.cpp
        src/play/spaceship_handler.h
        src/play/ui/setup_ui.cpp
        src/play/ui/setup_ui.h
        src/play/crew_factory.h
        src/play/crew_factory.cpp
        src/play/space_map.cpp
        src/play/space_map.h
        src/play/spaceship_factory.cpp
        src/play/spaceship_factory.h src/play/ui/running_ui.cpp src/play/ui/running_ui.h src/play/ui/hud_display.cpp src/play/ui/hud_display.h src/play/nav_control_manager.cpp src/play/nav_control_manager.h src/play/flying_debris.cpp src/play/flying_debris.h src/play/pause_menu.cpp src/play/pause_menu.h src/play/situation_type.h)

add_executable(sc ${MAIN} ${SOURCE_FILES})
add_executable(sc_test ${SOURCE_FILES} ${TEST_FILES})

target_link_libraries(sc spdlog::spdlog menu ncurses effolkronium_random)
target_link_libraries(sc_test spdlog::spdlog menu ncurses Catch2::Catch2 effolkronium_random)