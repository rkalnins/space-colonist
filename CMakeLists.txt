cmake_minimum_required(VERSION 3.14)
project(sc)


set(CMAKE_CXX_STANDARD 20)

find_package(Curses REQUIRED)
find_package(spdlog REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})

set(SOURCE_FILES src/main.cpp)

add_executable(sc ${SOURCE_FILES})
target_link_libraries(sc spdlog::spdlog ${CURSES_LIBRARIES})