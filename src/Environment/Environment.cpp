#include <Environment/Environment.hpp>

#include <iostream>
#include <Properties.hpp>
#include <Entities/ControllableEntity.hpp>
#include <Util/JsonFile.hpp>
#include <Util/Schemas.hpp>

Environment::Environment() {
    camera.setSize(Properties::ScreenWidth, Properties::ScreenHeight);
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

    camera.zoom(0.5f);
}

Environment::Environment(const std::string& file) {
    JsonFile input(Properties::EnvironmentFilePath+file);
    if (!Schemas::environmentFileSchema().validate(input, true)) {
        std::cerr << "Leaving environment empty on failed load" << std::endl;
        player = ControllableEntity::createPlayer({0, 0}, {0, 0});
        entities.push_back(player);
        return;
    }
    const JsonGroup& data = input.getRoot();

    name = *data.getField("name")->getAsString();
    victoryRegion.top = *data.getField("winZone")->getAsGroup()->getField("top")->getAsNumeric();
    victoryRegion.left = *data.getField("winZone")->getAsGroup()->getField("left")->getAsNumeric();
    victoryRegion.width = *data.getField("winZone")->getAsGroup()->getField("width")->getAsNumeric();
    victoryRegion.height = *data.getField("winZone")->getAsGroup()->getField("height")->getAsNumeric();
    bounds.left = bounds.top = 0;
    bounds.width = *data.getField("width")->getAsNumeric();
    bounds.height = *data.getField("height")->getAsNumeric();

    const JsonGroup& spawn = *data.getField("playerSpawn")->getAsGroup();
    player = ControllableEntity::createPlayer(
        {
            *spawn.getField("x")->getAsNumeric(),
            *spawn.getField("y")->getAsNumeric()
        },
        {0, 0}
    );
    entities.push_back(player);

    const JsonList& entityList = *data.getField("entities")->getAsList();
    for (unsigned int i = 0; i<entityList.size(); ++i) {
        entities.push_back(Entity::create(*entityList[i].getAsGroup()));
    }

    background.load(*data.getField("background")->getAsGroup());
}

void Environment::update(float dt) {
    const sf::FloatRect region(
        camera.getCenter() - camera.getSize()/2.0f,
        camera.getSize()
    );
    background.update(region);
    for (Entity::Ptr entity : entities) {
        for (Entity::Ptr gravEnt : entities) {
            gravEnt->applyGravityToEntity(entity);
        }
        entity->update(dt);
    }

    camera.setCenter(player->getPosition());
    camera.setRotation(player->getRotation());
}

void Environment::render(sf::RenderTarget& target) {
    target.setView(camera);

    background.render(target);

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