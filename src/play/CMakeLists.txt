add_subdirectory(situation)
add_subdirectory(ui)

set(PLAY_SRC
        ${CMAKE_CURRENT_LIST_DIR}/crew_factory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/crew_member.cpp
        ${CMAKE_CURRENT_LIST_DIR}/flying_debris.cpp
        ${CMAKE_CURRENT_LIST_DIR}/item.cpp
        ${CMAKE_CURRENT_LIST_DIR}/nav_control_manager.cpp
        ${CMAKE_CURRENT_LIST_DIR}/space_map.cpp
        ${CMAKE_CURRENT_LIST_DIR}/spaceship.cpp
        ${CMAKE_CURRENT_LIST_DIR}/spaceship_factory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/spaceship_handler.cpp)

target_sources(sc PRIVATE ${PLAY_SRC})
target_sources(sc_test PRIVATE ${PLAY_SRC})
