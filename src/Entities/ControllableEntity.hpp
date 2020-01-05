#ifndef CONTROLLABLEENTITY_HPP
#define CONTROLLABLEENTITY_HPP

#include <Entities/Entity.hpp>
#include <Entities/EntityController.hpp>

/**
 * Entity class for all controllable entity types, such as player and ai
 */
class ControllableEntity : public Entity {
public:
    /**
     * Creates an instance of ControllableEntity
     * 
     * \see Entity::Entity
     * \param controller The controller to use
     */
    static Entity::Ptr create(
        const std::string& name, const std::string& animFile, 
        const sf::Vector2f& position, const sf::Vector2f& velocity,
        float mass, bool canMove, bool hasGravity, float gRange,
        EntityController::Ptr controller
    );

    /**
     * Helper function to create a Player entity
     */
    static Entity::Ptr createPlayer(const sf::Vector2f& position, const sf::Vector2f& velocity);

    virtual ~ControllableEntity() = default;

private:
    EntityController::Ptr controller;

    ControllableEntity(const std::string& name, const std::string& animFile, 
                       const sf::Vector2f& position, const sf::Vector2f& velocity,
                       float mass, bool canMove, bool hasGravity, float gRange,
                       EntityController::Ptr controller);

    virtual void customUpdateLogic(float dt) override;
};

#endif