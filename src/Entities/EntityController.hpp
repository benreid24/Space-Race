#ifndef ENTITYCONTROLLER_HPP
#define ENTITYCONTROLLER_HPP

#include <memory>

class Entity;

/**
 * Base class for anything that can control an entity (player, ai, etc)
 */
class EntityController {
public:
    typedef std::shared_ptr<EntityController> Ptr;

    virtual ~EntityController() = default;

    /**
     * Apply accelaeration to the entity as per controls
     */
    virtual void update(Entity* entity, float dt) = 0;
};

#endif