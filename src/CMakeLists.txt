add_subdirectory(loop_control)
add_subdirectory(play)
add_subdirectory(windows)

add_subdirectory(test)

target_sources(sc PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/game.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp)

target_sources(sc_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/game.cpp)