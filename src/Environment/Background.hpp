#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <Environment/Backgrounds/BackgroundElementGenerator.hpp>
#include <Util/JsonFile.hpp>

/**
 * Represents the background of an Environment
 */
class Background {
public:
    void load(const JsonGroup& data);

    void update(const sf::FloatRect& activeRegion);

    void render(sf::RenderTarget& target);

private:
    sf::Color color;
    std::vector<BackgroundElementGenerator::Ptr> generators;
};

#endif