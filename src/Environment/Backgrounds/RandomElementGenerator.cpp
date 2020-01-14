#include <Environment/Backgrounds/RandomElementGenerator.hpp>
#include <Util/Util.hpp>

RandomElementGenerator::RandomElementGenerator(
    const std::string& gfx, float density, bool preserveAR,
    const sf::Vector2f& minScale, const sf::Vector2f& maxScale)
: BackgroundElementGenerator(gfx, preserveAR, minScale, maxScale), density(density) {}

BackgroundElementGenerator::ElementBucket RandomElementGenerator::generate(const sf::FloatRect& region) {
    BackgroundElementGenerator::ElementBucket elements;
    const float eArea = getElementSize().x * getElementSize().y;
    const float gArea = region.width * region.height;
    const unsigned int n = gArea / eArea * density;

    for (unsigned int i = 0; i<n; ++i) {
        elements.push_back({
            {
                randomFloat(region.left, region.left + region.width),
                randomFloat(region.top, region.top + region.height)
            },
            getElementScale()
        });
    }

    return elements;
}