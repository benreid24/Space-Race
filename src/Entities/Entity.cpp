#include <Entities/Entity.hpp>

#include <cmath>
#include <Properties.hpp>
#include <Util/ResourcePool.hpp>
#include <Util/Schemas.hpp>
#include <Entities/MotionTypes/PhysicsMotion.hpp>

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
, motion(PhysicsMotion::create(position, velocity))
, mass(mass)
, gravitationalRange(gRange <= 0 ? std::sqrt(Properties::GravitationalConstant * mass)/minAccel : gRange)
, gRangeSqrd(gravitationalRange * gravitationalRange)
, minGravDist((animation.getSize().x + animation.getSize().y)/2)
, canMove(canMove), hasGravity(hasGravity)
{
    motion->setMotionEnabled(canMove);
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

Entity::Ptr Entity::create(const JsonGroup& data) {
    if (!Schemas::entitySchema().validate(data, true))
        return nullptr;

    float gRange = 0;
    if (data.hasField("gravityRange"))
        gRange = *data.getField("gravityRange")->getAsNumeric();
    return Entity::create(
        *data.getField("name")->getAsString(),
        *data.getField("gfx")->getAsString(),
        {
            *data.getField("x")->getAsNumeric(),
            *data.getField("y")->getAsNumeric()
        },
        {
            *data.getField("vx")->getAsNumeric(),
            *data.getField("vy")->getAsNumeric()
        },
        *data.getField("mass")->getAsNumeric(),
        *data.getField("canMove")->getAsBool(),
        *data.getField("hasGravity")->getAsBool(),
        gRange
    );
}

void Entity::update(float dt) {
    customUpdateLogic(dt);
    animation.update();

    motion->update(this, dt);

    rotation += rotationRate * dt;
    rotationRate = 0;
    strongestGravity.magnitude = 0;
    parentBody = nullptr;
}

const std::string& Entity::getName() const {
    return name;
}

sf::FloatRect Entity::getBoundingBox() const {
    return sf::FloatRect(
        motion->getPosition() - animation.getSize() / 2.0f,
        animation.getSize()
    );
}

float Entity::distanceToSquared(const sf::Vector2f& pos) const {
    const float dx = motion->getPosition().x - pos.x;
    const float dy = motion->getPosition().y - pos.y;
    return dx*dx + dy*dy;
}

float Entity::getRotation() const {
    return rotation;
}

void Entity::applyRotation(float rate) {
    rotationRate += rate;
}

const sf::Vector2f& Entity::getPosition() const {
    return motion->getPosition();
}

const sf::Vector2f& Entity::getVelocity() const {
    return motion->getVelocity();
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

    const float dx = motion->getPosition().x - pos.x;
    const float dy = motion->getPosition().y - pos.y;
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

void Entity::applyGravityToEntity(Entity::Ptr entity) {
    if (hasGravity && this != entity.get()) {
        if (distanceToSquared(entity->getPosition()) <= gRangeSqrd) {
            const sf::Vector2f gravity = getGravitationalAcceleration(entity);
            entity->applyAcceleration(gravity);
            const AngularVectorF rGrav(gravity);
            if (entity->strongestGravity.magnitude < rGrav.magnitude) {
                entity->strongestGravity = rGrav;
                entity->parentBody = shared_from_this();
            }
        }
    }
}

Entity::Ptr Entity::currentParentBody() const {
    return parentBody;
}

void Entity::changeMotionType(EntityMotion::Ptr newMotion) {
    motion = newMotion;
    motion->setMotionEnabled(canMove);
}

void Entity::applyForce(const sf::Vector2f& force) {
    applyAcceleration(force / mass);
}

void Entity::applyAcceleration(const sf::Vector2f& a) {
    motion->applyAcceleration(a);
}

void Entity::render(sf::RenderTarget& target) {
    if (hasGravity) {
        const float gRange = getGravitationalRange();
        const float darkestBlue = 255 - std::min(mass / 10000.0f * 40.0f, 255.0f);
        sf::VertexArray circle(sf::PrimitiveType::TriangleFan, 362);
        circle[0].position = motion->getPosition();
        circle[0].color = sf::Color(0, 0, darkestBlue, 130);
        for (unsigned int i = 1; i<362; ++i) {
            circle[i].position.x = motion->getPosition().x + gRange * std::cos(float(i) / 180 * 3.1415);
            circle[i].position.y = motion->getPosition().y + gRange * std::sin(float(i) / 180 * 3.1415);
            circle[i].color = sf::Color(100, 100, 255, 30);
        }
        target.draw(circle);
    }

    animation.setPosition(motion->getPosition());
    animation.setRotation(rotation);
    animation.draw(target);
}
