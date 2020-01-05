#ifndef PLAYERCONTROLLER_HPP
#define PLAYERCONTROLLER_HPP

#include <Entities/EntityController.hpp>

/**
 * EntityController that takes player input and handles fuel usage
 */
class PlayerController : public EntityController {
public:
    virtual ~PlayerController() = default;

    static EntityController::Ptr create();

    virtual void update(Entity* entity, float dt) override;

private:
    //TODO - fuel, controls, etc

    PlayerController();
};

#endif