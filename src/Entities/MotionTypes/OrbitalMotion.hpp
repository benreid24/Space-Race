#ifndef ORBITALMOTION_HPP
#define ORBITALMOTION_HPP

#include <Entities/Entity.hpp>
#include <Entities/EntityMotion.hpp>

/**
 * Orbits around a given entity in a simple circle
 */
class OrbitalMotion : public EntityMotion {
public:
    /**
     * Creates a new motion object for the given parent and satellite
     */
    static EntityMotion::Ptr create(Entity::Ptr parentBody, Entity* satellite);

    virtual ~OrbitalMotion() = default;

    virtual void applyAcceleration(const sf::Vector2f&) override {}

    /**
     * Positions the satellite at the proper phase and relative position
     */
    virtual void update(Entity* entity, float dt) override;

private:
    Entity::Ptr parentBody;
    float elapsedTime;

    const float radius;
    const float orbitalVelocity;
    const float period;
    const float insertionAngle;
    const bool clockwise;

    OrbitalMotion(Entity::Ptr parentBody, Entity* satellite);

    float getCurrentAngle() const;
    sf::Vector2f getRelativePosition() const;
    AngularVectorF getCurrentVelocity() const;
};

#endif