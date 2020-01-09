#include <Entities/MotionTypes/OrbitalMotion.hpp>

#include <cmath>
#include <Properties.hpp>
#include <iostream>

namespace {
bool isOrbitClockwise(float angle, const sf::Vector2f& velocity) {
    const AngularVectorF angVel(velocity);
    const float cw = (angVel.angle > angle) ? (angVel.angle - angle) : (angVel.angle + 360 - angle);
    const float ccw = (angle > angVel.angle) ? (angle - angVel.angle) : (angle + 360 - angVel.angle);
    return cw < ccw;
}
}

EntityMotion::Ptr OrbitalMotion::create(Entity::Ptr parentBody, Entity* satellite) {
    return EntityMotion::Ptr(new OrbitalMotion(parentBody, satellite));
}

OrbitalMotion::OrbitalMotion(Entity::Ptr parentBody, Entity* satellite)
: EntityMotion(satellite->getPosition())
, parentBody(parentBody)
, elapsedTime(0)
, radius(std::sqrt(parentBody->distanceToSquared(satellite->getPosition())))
, orbitalVelocity(std::sqrt(Properties::GravitationalConstant * parentBody->getMass() / radius))
, period((radius * 2 * 3.1415926) / orbitalVelocity)
, insertionAngle(AngularVectorF(satellite->getPosition()-parentBody->getPosition()).angle)
, clockwise(isOrbitClockwise(insertionAngle, satellite->getVelocity()))
{
    setVelocity(getCurrentVelocity().toCartesian());
}

float OrbitalMotion::getCurrentAngle() const {
    const float passedOrbits = elapsedTime / period;
    const float direction = (clockwise) ? (1) : (-1);
    return insertionAngle + passedOrbits * direction * 360;
}

AngularVectorF OrbitalMotion::getCurrentVelocity() const {
    const float offset = clockwise ? 90 : -90;
    return AngularVectorF(orbitalVelocity, getCurrentAngle() + offset);
}

sf::Vector2f OrbitalMotion::getRelativePosition() const {
    return AngularVectorF(radius, getCurrentAngle()).toCartesian();
}

void OrbitalMotion::update(Entity*, float dt) {
    elapsedTime += dt;
    setPosition(parentBody->getPosition() + getRelativePosition());
    setVelocity(getCurrentVelocity().toCartesian());
}