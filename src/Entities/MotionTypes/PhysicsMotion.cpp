#include <Entities/MotionTypes/PhysicsMotion.hpp>

EntityMotion::Ptr PhysicsMotion::create(const sf::Vector2f& pos, const sf::Vector2f& vel) {
    return EntityMotion::Ptr(new PhysicsMotion(pos, vel));
}

EntityMotion::Ptr PhysicsMotion::create(Entity::Ptr entity) {
    return EntityMotion::Ptr(new PhysicsMotion(entity->getPosition(), entity->getVelocity()));
}

PhysicsMotion::PhysicsMotion(const sf::Vector2f& pos, const sf::Vector2f& vel)
: EntityMotion(pos), acceleration(0,0) {
    setVelocity(vel);
}

void PhysicsMotion::applyAcceleration(const sf::Vector2f& a) {
    acceleration += a;
}

void PhysicsMotion::update(Entity*, float dt) {
    sf::Vector2f v = getVelocity();
    sf::Vector2f p = getPosition();

    p.x += v.x*dt + acceleration.x*dt*dt/2;
    p.y += v.y*dt + acceleration.y*dt*dt/2;
    v += acceleration * dt;

    setPosition(p);
    setVelocity(v);
    
    acceleration.x = acceleration.y = 0;
}