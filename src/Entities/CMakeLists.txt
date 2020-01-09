target_sources(SpaceRace PUBLIC
    Entity.hpp
    Entity.cpp
    EntityController.hpp
    EntityMotion.hpp
    ControllableEntity.hpp
    ControllableEntity.cpp
)

add_subdirectory(Controllers)
add_subdirectory(MotionTypes)