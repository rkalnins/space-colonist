
set(LOOP_SRC
        ${CMAKE_CURRENT_LIST_DIR}/task.cpp
        ${CMAKE_CURRENT_LIST_DIR}/task_pool.cpp)

target_sources(sc PRIVATE ${LOOP_SRC})
target_sources(sc_test PRIVATE ${LOOP_SRC})