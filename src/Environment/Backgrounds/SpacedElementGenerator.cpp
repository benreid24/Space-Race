#include <Environment/Backgrounds/SpacedElementGenerator.hpp>
#include <Util/Util.hpp>
#include <cmath>

#include <iostream>

SpacedElementGenerator::SpacedElementGenerator(
        const std::string& file, bool preserveAR,
        const sf::Vector2f& minScale, const sf::Vector2f& maxScale,
        const sf::Vector2f& minSpace, const sf::Vector2f& maxSpace)
: BackgroundElementGenerator(file, preserveAR, minScale, maxScale)
, minSpace(minSpace)
, maxSpace(maxSpace) {}

BackgroundElementGenerator::ElementBucket SpacedElementGenerator::generate(
                                                const sf::FloatRect& region) {
    const sf::Vector2f avgSpace = (maxSpace+minSpace)/2.0f;
    const sf::Vector2f avgSize = avgSpace + getElementSize();
    const sf::Vector2f offset = {
        static_cast<float>(static_cast<int>(region.width) % static_cast<int>(avgSize.x)) / 2.0f,
        static_cast<float>(static_cast<int>(region.height) % static_cast<int>(avgSize.y)) / 2.0f
    };
    const sf::Vector2i n = {
        static_cast<int>(std::floor(region.width / avgSize.x)),
        static_cast<int>(std::floor(region.height / avgSize.y))
    };

    BackgroundElementGenerator::ElementBucket elements;

    const float halfY = (maxSpace.y - minSpace.y) / 2.0f;
    float x = region.left + offset.x;
    float y = region.top + offset.y;
    for (int cy = 0; cy<n.y; ++cy) {
        for (int cx = 0; cx<n.x; ++cx) {
            const float ox = randomFloat(minSpace.x, maxSpace.x);
            const float oy = randomFloat(minSpace.y, maxSpace.y) - halfY;
            elements.push_back({
                sf::Vector2f(x,y+oy),
                getElementScale()
            });
            x += getElementSize().x + ox;
        }
        y += avgSize.y;
    }

    return elements;
}