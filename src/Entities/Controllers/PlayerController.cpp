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
    const float a = 1000;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        entity->applyAcceleration({0, -a});
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        entity->applyAcceleration({a, 0});
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        entity->applyAcceleration({0, a});
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        entity->applyAcceleration({-a, 0});

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
        entity->reset({250, 800});
}