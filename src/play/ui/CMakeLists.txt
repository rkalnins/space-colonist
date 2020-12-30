
set(UI_SRC
        ${CMAKE_CURRENT_LIST_DIR}/hud_display.cpp
        ${CMAKE_CURRENT_LIST_DIR}/pause_menu.cpp
        ${CMAKE_CURRENT_LIST_DIR}/running_ui.cpp
        ${CMAKE_CURRENT_LIST_DIR}/setup_ui.cpp
        ${CMAKE_CURRENT_LIST_DIR}/store_ui.cpp
        )

target_sources(sc PRIVATE ${UI_SRC})
target_sources(sc_test PRIVATE ${UI_SRC})