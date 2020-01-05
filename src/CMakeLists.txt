add_executable(SpaceRace
    main.cpp
    Properties.hpp
    Properties.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_definitions(-DGAME)

target_link_libraries(SpaceRace
    sfml-graphics
    sfml-window
    sfml-network
    sfml-audio
    sfml-system
    sfml-main
)

add_subdirectory(Entities)
add_subdirectory(Environment)
add_subdirectory(Media)
add_subdirectory(Util)

install(TARGETS SpaceRace DESTINATION ${PROJECT_SOURCE_DIR})
if (WIN32)
    install(FILES ${CMAKE_BINARY_DIR}/bin/openal32.dll DESTINATION ${PROJECT_SOURCE_DIR})
endif()