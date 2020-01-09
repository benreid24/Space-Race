#ifndef ENTITYMOTION_HPP
#define ENTITYMOTION_HPP

#include <memory>
#include <SFML/Graphics.hpp>

class Entity;

/**
 * Motion provider base class for Entity objects
 */
class EntityMotion {
public:
    typedef std::shared_ptr<EntityMotion> Ptr;

    /**
     * Create the motion object from position
     */
    EntityMotion(const sf::Vector2f& position) : canMove(true), pos(position), vel(0,0) {}

    virtual ~EntityMotion() = default;

    /**
     * Apply an acceleration
     */
    virtual void applyAcceleration(const sf::Vector2f& a) = 0;

    /**
     * Update position and velocity
     */
    virtual void update(Entity* entity, float dt) = 0;

    /**
     * Returns the position of the Entity
     */
    const sf::Vector2f& getPosition() const { return pos; }

    /**
     * Returns the velocity of the Entity
     */
    const sf::Vector2f& getVelocity() const { return vel; }

    /**
     * Set whether or not motion is enabled. Position and velocity are constant if disabled
     */
    void setMotionEnabled(bool enabled) { canMove = enabled; }

protected:
    void setPosition(const sf::Vector2f& p) { if (canMove) pos = p; }
    void setVelocity(const sf::Vector2f& v) { if (canMove) vel = v; }

private:
    bool canMove;
    sf::Vector2f pos;
    sf::Vector2f vel;
};

#endif