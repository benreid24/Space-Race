cmake_minimum_required(VERSION 3.15)

project(SpaceRace C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_INSTALL_PREFIX .)
set(BUILD_SHARED_LIBS Off)
set(SFML_USE_STATIC_STD_LIBS On)

include_directories(lib/SFML/include)
add_definitions(-DSFML_STATIC -DSFGUI_STATIC)

add_subdirectory(lib/SFML)
add_subdirectory(src)