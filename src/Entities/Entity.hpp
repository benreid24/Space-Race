#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <memory>
#include <list>

#include <SFML/Graphics.hpp>

#include <Entities/EntityMotion.hpp>
#include <Media/Animation.hpp>
#include <Util/ResourceTypes.hpp>
#include <Util/AngularVector.hpp>
#include <Util/JsonFile.hpp>

/**
 * Base class for any object that can exist in the Environment
 */
class Entity : public std::enable_shared_from_this<Entity> {
public:
    typedef std::shared_ptr<Entity> Ptr;

    /**
     * Creates an Entity
     * 
     * \param name The name of the Entity
     * \param animFile The file to load animation from
     * \param position The global position to spawn at
     * \param velocity The starting velocity
     * \param mass The mass of the entity. Determines gravitational pull and range
     * \param canMove True if moveable and can be affected force/acceleration
     * \param hasGravity Whether or not the Entity emits gravity
     * \param gRange Optional maximum limit of gravitational range. Calculated from mass by default
     */
    static Ptr create(
        const std::string& name, const std::string& animFile, const sf::Vector2f& position,
        const sf::Vector2f& velocity, float mass, bool canMove, bool hasGravity, float gRange = -1
    );

    /**
     * Creates an Entity from json data
     */
    static Ptr create(const JsonGroup& data);

    virtual ~Entity() = default;

    const std::string& getName() const;
    sf::FloatRect getBoundingBox() const;
    float distanceToSquared(const sf::Vector2f& position) const;

    float getRotation() const;
    void applyRotation(float rate);

    const sf::Vector2f& getPosition() const;
    const sf::Vector2f& getVelocity() const;
    float getMass() const;

    float getGravitationalRange() const;
    sf::Vector2f getGravitationalAcceleration(const sf::Vector2f& position) const;
    sf::Vector2f getGravitationalAcceleration(Ptr entity) const;

    void applyGravityToEntity(Ptr entity);
    Ptr currentParentBody() const;

    void changeMotionType(EntityMotion::Ptr motion);
    void applyForce(const sf::Vector2f& force);
    void applyAcceleration(const sf::Vector2f& acceleration);

    /**
     * Applies forces/accelerations to velocity and velocity to position over time
     */
    void update(float dt);

    /**
     * Renders to the target. Position is in pixels. sf::View should be used for camera
     */
    void render(sf::RenderTarget& target);

protected:
    Entity(const std::string& name, const std::string& animFile, const sf::Vector2f& position,
           const sf::Vector2f& velocity, float mass, bool canMove, bool hasGravity, float gRange = -1);

    /**
     * Called from update() for derived classes to implement custom functionality
     */
    virtual void customUpdateLogic(float dt) {}

private:
    const std::string name;
    AnimationReference animSrc;
    Animation animation;

    float rotation;
    float rotationRate; //TODO - persistent rotation rate

    EntityMotion::Ptr motion;

    const float mass;
    const float gravitationalRange, gRangeSqrd;
    const float minGravDist;
    const bool canMove;
    const bool hasGravity;

    Ptr parentBody;
    AngularVectorF strongestGravity;
};

/**
 * IDEAS:
 *      - EntityVisual: Wrapper over rendering
 *      - EntityMovement: Wrapper over movement. Physics, on-rails, etc
 */

#endif