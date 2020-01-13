#ifndef RANDOMELEMENTGENERATOR_HPP
#define RANDOMELEMENTGENERATOR_HPP

#include <Environment/Backgrounds/BackgroundElementGenerator.hpp>

struct ElementGeneratorFactory;

/**
 * Generates elements randomly with a given density
 */
class RandomElementGenerator : public BackgroundElementGenerator {
    RandomElementGenerator(const std::string& gfx, float density,
        const sf::Vector2f& minScale, const sf::Vector2f& maxScale);

    virtual BackgroundElementGenerator::ElementBucket generate(const sf::FloatRect& region) override;

private:
    const float density;

    friend class ElementGeneratorFactory;
};

#endif