#include <Entities/Controllers/PlayerController.hpp>

#include <cmath>
#include <SFML/System.hpp>
#include <Entities/Entity.hpp>

EntityController::Ptr PlayerController::create() {
    return EntityController::Ptr(new PlayerController());
}

PlayerController::PlayerController() {
    //TODO - load/init controls
}

void PlayerController::update(Entity* entity, float dt) {
    const sf::Vector2f& v = entity->getVelocity();
    float angle = std::atan2(v.y, v.x);
    const float a = 10;
    bool move = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        move = true;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        move = true;
        angle += 90 / 180 * 3.1415;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        move = true;
        angle += 180 / 180 * 3.1415;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        move = true;
        angle += 270 / 180 * 3.1415;
    }

    if (move) {
        entity->applyAcceleration({
            a * std::cos(angle),
            a * std::sin(angle)
        });
    }
}