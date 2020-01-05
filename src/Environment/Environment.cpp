#include <Environment/Environment.hpp>

#include <Properties.hpp>
#include <Entities/ControllableEntity.hpp>

Environment::Environment() {
    camera.setSize(Properties::ScreenWidth, Properties::ScreenHeight);
    background = sf::Color::Black;
    victoryRegion = {0, 0, 800, 100};
    player = ControllableEntity::createPlayer({250, 800}, {0, 0});
    entities.push_back(player);
    entities.push_back(Entity::create(
        "Earth", "Planets/earth.anim", {500, 500},
        {0, 0}, 10000, false, true
    ));
    entities.push_back(Entity::create(
        "NeutronEarth", "Planets/earth.anim", {800, 900},
        {0, 0}, 100000, false, true, 200
    ));
}

Environment::Environment(File& file) {
    // TODO
}

void Environment::update(float dt) {
    for (Entity::Ptr entity : entities) {
        for (Entity::Ptr gravEnt : entities) {
            if (entity.get() != gravEnt.get())
                entity->applyAcceleration(gravEnt->getGravitationalAcceleration(entity));
        }
        entity->update(dt);
    }
}

void Environment::render(sf::RenderTarget& target) {
    camera.setCenter(player->getPosition());
    target.setView(camera);

    target.clear(background);

    sf::RectangleShape rect({victoryRegion.width, victoryRegion.height});
    rect.setPosition({victoryRegion.left, victoryRegion.top});
    rect.setFillColor(sf::Color(0, 200, 0, 128));
    rect.setOutlineColor(sf::Color::Green);
    rect.setOutlineThickness(1);
    target.draw(rect);

    for (Entity::Ptr entity : entities) {
        entity->render(target);
    }
}

Environment::PlayerStatus Environment::getPlayerStatus() const {
    if (victoryRegion.intersects(player->getBoundingBox()))
        return PlayerStatus::Won;
    return PlayerStatus::Playing; //TODO - others
}