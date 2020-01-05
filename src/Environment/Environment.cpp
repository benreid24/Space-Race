#include <Environment/Environment.hpp>

#include <Entities/ControllableEntity.hpp>

Environment::Environment() {
    background = sf::Color::Black;
    player = ControllableEntity::createPlayer({250, 800}, {0, 0});
    entities.push_back(player);
}

Environment::Environment(File& file) {
    // TODO
}

void Environment::update(float dt) {
    for (Entity::Ptr entity : entities) {
        for (Entity::Ptr gravEnt : entities) {
            entity->applyAcceleration(gravEnt->getGravitationalAcceleration(entity));
        }
        entity->update(dt);
    }
}

void Environment::render(sf::RenderTarget& target) {
    target.clear(background);
    for (Entity::Ptr entity : entities) {
        entity->render(target);
    }
}

Environment::PlayerStatus Environment::getPlayerStatus() const {
    if (victoryRegion.intersects(player->getBoundingBox()))
        return PlayerStatus::Won;
    return PlayerStatus::Playing; //TODO - others
}