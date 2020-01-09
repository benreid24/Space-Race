#ifndef PHYSICSMOTION_HPP
#define PHYSICSMOTION_HPP

#include <Entities/Entity.hpp>
#include <Entities/EntityMotion.hpp>

/**
 * Standard physics model to provide Entity motion
 */
class PhysicsMotion : public EntityMotion {
public:
    /**
     * Create the motion object for the given Entity's current position and velocity
     */
    static EntityMotion::Ptr create(Entity::Ptr entity);

    /**
     * Create the motion object from position and velocity
     */
    static EntityMotion::Ptr create(const sf::Vector2f& pos, const sf::Vector2f& vel);

    virtual ~PhysicsMotion() = default;

    /**
     * Apply acceleration
     */
    virtual void applyAcceleration(const sf::Vector2f& a) override;

    /**
     * Updates velocity and position
     */
    virtual void update(Entity* entity, float dt) override;

private:
    sf::Vector2f acceleration;

    PhysicsMotion(const sf::Vector2f& pos, const sf::Vector2f& vel);
};

#endif