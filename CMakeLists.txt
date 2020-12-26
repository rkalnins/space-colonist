cmake_minimum_required(VERSION 3.14)
project(sc)


set(CMAKE_CXX_STANDARD 20)

find_package(spdlog REQUIRED)
find_package(Catch2 REQUIRED)
find_package(effolkronium_random REQUIRED)

include_directories(/usr/local/opt/ncurses/include)

link_directories(/usr/local/opt/ncurses/lib)

add_executable(sc)
add_executable(sc_test)

add_subdirectory(src)

target_link_libraries(sc spdlog::spdlog menu ncurses effolkronium_random)
target_link_libraries(sc_test spdlog::spdlog menu ncurses Catch2::Catch2 effolkronium_random)