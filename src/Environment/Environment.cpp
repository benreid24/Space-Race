#include <Environment/Environment.hpp>

Environment::Environment() {
    background = sf::Color::Black;
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