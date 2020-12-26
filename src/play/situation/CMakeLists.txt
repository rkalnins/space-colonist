
set(SIT_SRC
        ${CMAKE_CURRENT_LIST_DIR}/air_filter_failure.cpp
        ${CMAKE_CURRENT_LIST_DIR}/major_situation.cpp
        ${CMAKE_CURRENT_LIST_DIR}/minor_situation.cpp
        ${CMAKE_CURRENT_LIST_DIR}/situation.cpp
        ${CMAKE_CURRENT_LIST_DIR}/situation_factory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/situation_manager.cpp
        ${CMAKE_CURRENT_LIST_DIR}/standard_engine_failure.cpp)

target_sources(sc PRIVATE ${SIT_SRC})
target_sources(sc_test PRIVATE ${SIT_SRC})