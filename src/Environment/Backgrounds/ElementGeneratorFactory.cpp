#include <Environment/Backgrounds/ElementGeneratorFactory.hpp>
#include <Environment/Backgrounds/RandomElementGenerator.hpp>

BackgroundElementGenerator::Ptr ElementGeneratorFactory::create(const JsonGroup& data) {
    const std::string gfx = *data.getField("file")->getAsString();
    sf::Vector2f minScale, maxScale;

    const JsonGroup& scale = *data.getField("scale")->getAsGroup();
    if (scale.hasField("fixed")) {
        minScale.x = *scale.getField("fixed")->getAsGroup()->getField("x")->getAsNumeric();
        minScale.y = *scale.getField("fixed")->getAsGroup()->getField("y")->getAsNumeric();
        maxScale = minScale;
    }
    else {
        minScale.x = *scale.getField("ranged")->getAsGroup()->getField("minx")->getAsNumeric();
        minScale.y = *scale.getField("ranged")->getAsGroup()->getField("miny")->getAsNumeric();
        maxScale.x = *scale.getField("ranged")->getAsGroup()->getField("maxx")->getAsNumeric();
        maxScale.y = *scale.getField("ranged")->getAsGroup()->getField("maxy")->getAsNumeric();
    }

    const JsonGroup& pos = *data.getField("positioning")->getAsGroup();
    if (pos.hasField("fixedSpacing")) {
        //TODO - fixed spacing generator
        return nullptr;
    }
    else if (pos.hasField("rangedSpacing")) {
        //TODO - ranged spacing generator
    }
    else if (pos.hasField("random")) {
        const float density = *pos.getField("random")->getAsGroup()->getField("density")->getAsNumeric();
        return BackgroundElementGenerator::Ptr(
            new RandomElementGenerator(gfx, density, minScale, maxScale)
        );
    }
    return nullptr;
}