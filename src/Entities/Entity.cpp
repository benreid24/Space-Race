#include <Entities/Entity.hpp>

#include <cmath>
#include <Properties.hpp>
#include <Util/ResourcePool.hpp>

namespace {
const float minAccel = 1; // minimum for gravity to apply
}

Entity::Entity(const std::string& name, const std::string& animFile,
               const sf::Vector2f& position, const sf::Vector2f& velocity,
               float mass, bool canMove, bool hasGravity)
: name(name)
, animSrc(animPool.loadResource(Properties::EntityAnimationPath+animFile))
, position(position), velocity(velocity), mass(mass)
, gravitationalRange(std::sqrt(Properties::GravitationalConstant * mass)/minAccel)
, canMove(canMove), hasGravity(hasGravity)
{
    animation.setSource(animSrc);
}

void Entity::update(float dt) {
    const sf::Vector2f& vi = velocity;
    position.x += vi.x*dt + acceleration.x*dt*dt/2;
    position.y = vi.y*dt + acceleration.y*dt*dt/2;
    velocity += acceleration*dt;
    customUpdateLogic(dt);
    acceleration.x = acceleration.y = 0;
    animation.update();
}

const sf::Vector2f& Entity::getPosition() const {
    return position;
}

const sf::Vector2f& Entity::getVelocity() const {
    return velocity;
}

const sf::Vector2f& Entity::getAcceleration() const {
    return acceleration;
}

float Entity::getMass() const {
    return mass;
}

float Entity::getGravitationalRange() const {
    if (!hasGravity)
        return 0;
    return gravitationalRange;
}

sf::Vector2f Entity::getGravitationalAcceleration(const sf::Vector2f& pos) const {
    if (!hasGravity)
        return sf::Vector2f(0, 0);

    const float dx = std::abs(position.x - pos.x);
    const float dy = std::abs(position.y - pos.y);
    return sf::Vector2f(
        Properties::GravitationalConstant * mass / (dx * dx),
        Properties::GravitationalConstant * mass / (dy * dy)
    );
}

sf::Vector2f Entity::getGravitationalAcceleration(Entity::Ptr entity) const {
    return getGravitationalAcceleration(entity->getPosition());
}

void Entity::applyForce(const sf::Vector2f& force) {
    applyAcceleration(force / mass);
}

void Entity::applyAcceleration(const sf::Vector2f& a) {
    if (canMove)
        acceleration += a;
}

void Entity::render(sf::RenderTarget& target) {
    animation.draw(target);
}