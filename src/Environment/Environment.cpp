#include <Environment/Environment.hpp>

#include <iostream>
#include <Properties.hpp>
#include <Entities/ControllableEntity.hpp>
#include <Util/JsonFile.hpp>
#include <Util/JSON/JsonSchema.hpp>

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

    camera.zoom(0.5f);
}

Environment::Environment(const std::string& file) {
    JsonFile input(Properties::EnvironmentFilePath+file);
    if (!JsonSchema::environmentFileSchema().validate(input, true)) {
        std::cerr << "Leaving environment empty on failed load" << std::endl;
        return;
    }
    const JsonGroup& data = input.getRoot();

    name = *data.getField("name")->getAsString();
    victoryRegion.top = *data.getField("winZone")->getAsGroup()->getField("top")->getAsNumeric();
    victoryRegion.left = *data.getField("winZone")->getAsGroup()->getField("left")->getAsNumeric();
    victoryRegion.width = *data.getField("winZone")->getAsGroup()->getField("width")->getAsNumeric();
    victoryRegion.height = *data.getField("winZone")->getAsGroup()->getField("height")->getAsNumeric();

    const JsonGroup& color = *data.getField("background")->getAsGroup()->getField("color")->getAsGroup();
    background.r = *color.getField("red")->getAsNumeric();
    background.g = *color.getField("green")->getAsNumeric();
    background.b = *color.getField("blue")->getAsNumeric();

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
        const JsonGroup& entity = *entityList[i].getAsGroup();
        entities.push_back(Entity::create(
            *entity.getField("name")->getAsString(),
            *entity.getField("gfx")->getAsString(),
            {
                *entity.getField("x")->getAsNumeric(),
                *entity.getField("y")->getAsNumeric()
            },
            {
                *entity.getField("vx")->getAsNumeric(),
                *entity.getField("vy")->getAsNumeric()
            },
            *entity.getField("mass")->getAsNumeric(),
            *entity.getField("canMove")->getAsBool(),
            *entity.getField("hasGravity")->getAsBool(),
            *entity.getField("gravityRange")->getAsNumeric()
        ));
    }
}

void Environment::update(float dt) {
    for (Entity::Ptr entity : entities) {
        for (Entity::Ptr gravEnt : entities) {
            gravEnt->applyGravityToEntity(entity);
        }
        entity->update(dt);
    }
}

void Environment::render(sf::RenderTarget& target) {
    camera.setCenter(player->getPosition());
    camera.setRotation(player->getRotation());
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