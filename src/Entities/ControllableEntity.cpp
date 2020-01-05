#include <Entities/ControllableEntity.hpp>
#include <Entities/Controllers/PlayerController.hpp>

ControllableEntity::ControllableEntity(
    const std::string& name, const std::string& animFile, 
    const sf::Vector2f& position, const sf::Vector2f& velocity,
    float mass, bool canMove, bool hasGravity, float gRange,
    EntityController::Ptr controller)
: Entity(name, animFile, position, velocity, mass, canMove, hasGravity)
, controller(controller)
{
    // noop
}

Entity::Ptr ControllableEntity::create(
    const std::string& name, const std::string& animFile, 
    const sf::Vector2f& position, const sf::Vector2f& velocity,
    float mass, bool canMove, bool hasGravity, float gRange,
    EntityController::Ptr controller)
{
    return Entity::Ptr(new ControllableEntity(
        name, animFile, position, velocity, mass, canMove, hasGravity, gRange, controller
    ));
}

Entity::Ptr ControllableEntity::createPlayer(const sf::Vector2f& position, const sf::Vector2f& velocity) {
    return Entity::Ptr(new ControllableEntity(
        "Player", "Ships/ship.anim", position, velocity, 10, true, false, -1, PlayerController::create()
    ));
}

void ControllableEntity::customUpdateLogic(float dt) {
    controller->update(this, dt);
}