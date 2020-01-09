#include <Entities/Controllers/PlayerController.hpp>

#include <cmath>
#include <SFML/System.hpp>
#include <Entities/Entity.hpp>
#include <Util/AngularVector.hpp>

EntityController::Ptr PlayerController::create() {
    return EntityController::Ptr(new PlayerController());
}

PlayerController::PlayerController() {
    //TODO - load/init controls
}

void PlayerController::update(Entity* entity, float dt) {
    AngularVectorF a(1000, entity->getRotation());

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        entity->applyAcceleration(a.toCartesian());
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        entity->applyAcceleration(a.rotate(90).toCartesian());
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        entity->applyAcceleration(a.rotate(180).toCartesian());
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        entity->applyAcceleration(a.rotate(270).toCartesian());

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        entity->applyRotation(120);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        entity->applyRotation(-120);
}