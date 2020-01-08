#include <Entities/Entity.hpp>

#include <cmath>
#include <Properties.hpp>
#include <Util/ResourcePool.hpp>

#include <iostream>

namespace {
const float minAccel = 20; // minimum for gravity to apply
}

Entity::Entity(
    const std::string& name, const std::string& animFile,
    const sf::Vector2f& position, const sf::Vector2f& velocity,
    float mass, bool canMove, bool hasGravity, float gRange)
: name(name)
, animSrc(animPool.loadResource(Properties::EntityAnimationPath+animFile))
, animation(animSrc, true)
, rotation(0)
, rotationRate(0)
, position(position), velocity(velocity), mass(mass)
, gravitationalRange(gRange <= 0 ? std::sqrt(Properties::GravitationalConstant * mass)/minAccel : gRange)
, gRangeSqrd(gravitationalRange * gravitationalRange)
, minGravDist((animation.getSize().x + animation.getSize().y)/2)
, canMove(canMove), hasGravity(hasGravity)
{
    // noop
}

Entity::Ptr Entity::create(
    const std::string& name, const std::string& animFile,
    const sf::Vector2f& position, const sf::Vector2f& velocity,
    float mass, bool canMove, bool hasGravity, float gRange
) {
    return Entity::Ptr(new Entity(
        name, animFile, position, velocity, mass, canMove, hasGravity, gRange
    ));
}

void Entity::update(float dt) {
    customUpdateLogic(dt);

    const sf::Vector2f& vi = velocity;
    position.x += vi.x*dt + acceleration.x*dt*dt/2;
    position.y += vi.y*dt + acceleration.y*dt*dt/2;
    velocity += acceleration * dt;

    rotation += rotationRate * dt;
    rotationRate = 0;
    
    acceleration.x = acceleration.y = 0;
    animation.update();
}

const std::string& Entity::getName() const {
    return name;
}

sf::FloatRect Entity::getBoundingBox() const {
    return sf::FloatRect(
        position - animation.getSize() / 2.0f,
        animation.getSize()
    );
}

float Entity::distanceToSquared(const sf::Vector2f& pos) const {
    const float dx = position.x - pos.x;
    const float dy = position.y - pos.y;
    return dx*dx + dy*dy;
}

float Entity::getRotation() const {
    return rotation;
}

void Entity::applyRotation(float rate) {
    rotationRate += rate;
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
    if (!hasGravity || distanceToSquared(pos) > gRangeSqrd)
        return sf::Vector2f(0, 0);

    const float dx = position.x - pos.x;
    const float dy = position.y - pos.y;
    const float angle = std::atan2(dy, dx);
    const float distSqrd = std::max(dx*dx + dy*dy, minGravDist*minGravDist);
    const float accel = Properties::GravitationalConstant * mass / distSqrd;

    return sf::Vector2f(
        accel * std::cos(angle),
        accel * std::sin(angle)
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
    if (hasGravity) {
        const float gRange = getGravitationalRange();
        const float darkestBlue = 255 - std::min(mass / 10000.0f * 40.0f, 255.0f);
        sf::VertexArray circle(sf::PrimitiveType::TriangleFan, 362);
        circle[0].position = position;
        circle[0].color = sf::Color(0, 0, darkestBlue, 130);
        for (unsigned int i = 1; i<362; ++i) {
            circle[i].position.x = position.x + gRange * std::cos(float(i) / 180 * 3.1415);
            circle[i].position.y = position.y + gRange * std::sin(float(i) / 180 * 3.1415);
            circle[i].color = sf::Color(100, 100, 255, 30);
        }
        target.draw(circle);
    }

    animation.setPosition(position);
    animation.setRotation(rotation);
    animation.draw(target);
}

void Entity::reset(const sf::Vector2f& pos) {
    position = pos;
    velocity = {0, 0};
}