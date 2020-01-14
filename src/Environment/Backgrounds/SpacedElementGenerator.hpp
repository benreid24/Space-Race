#ifndef SPACEDELEMENTGENERATOR_HPP
#define SPACEDELEMENTGENERATOR_HPP

#include <Environment/Backgrounds/BackgroundElementGenerator.hpp>

class ElementGeneratorFactory;

/**
 * Generates background elements with random or uniform spacing
 */
class SpacedElementGenerator : public BackgroundElementGenerator {
    SpacedElementGenerator(const std::string& file, bool preserveAR,
        const sf::Vector2f& minScale, const sf::Vector2f& maxScale,
        const sf::Vector2f& minSpace, const sf::Vector2f& maxSpace);

    virtual BackgroundElementGenerator::ElementBucket generate(const sf::FloatRect& region) override;

private:
    const sf::Vector2f minSpace;
    const sf::Vector2f maxSpace;

    friend class ElementGeneratorFactory;
};

#endif