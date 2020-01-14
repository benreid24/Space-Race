#include <Environment/Backgrounds/ElementGeneratorFactory.hpp>
#include <Environment/Backgrounds/RandomElementGenerator.hpp>
#include <Environment/Backgrounds/SpacedElementGenerator.hpp>

BackgroundElementGenerator::Ptr ElementGeneratorFactory::create(const JsonGroup& data) {
    const std::string gfx = *data.getField("file")->getAsString();
    bool preserveAR = false;
    sf::Vector2f minScale, maxScale;

    const JsonGroup& scale = *data.getField("scale")->getAsGroup();
    if (scale.hasField("fixed")) {
        minScale.x = *scale.getField("fixed")->getAsGroup()->getField("x")->getAsNumeric();
        minScale.y = *scale.getField("fixed")->getAsGroup()->getField("y")->getAsNumeric();
        maxScale = minScale;
    }
    else {
        if (scale.getField("ranged")->getAsGroup()->hasField("preserveAR"))
            preserveAR = *scale.getField("ranged")->getAsGroup()->getField("preserveAR")->getAsBool();
        minScale.x = *scale.getField("ranged")->getAsGroup()->getField("minx")->getAsNumeric();
        minScale.y = *scale.getField("ranged")->getAsGroup()->getField("miny")->getAsNumeric();
        maxScale.x = *scale.getField("ranged")->getAsGroup()->getField("maxx")->getAsNumeric();
        maxScale.y = *scale.getField("ranged")->getAsGroup()->getField("maxy")->getAsNumeric();

        if (scale.getField("ranged")->getAsGroup()->hasField("allowFlipH")) {
            if (*scale.getField("ranged")->getAsGroup()->getField("allowFlipH")->getAsBool())
                minScale.x *= -1;
        }
        if (scale.getField("ranged")->getAsGroup()->hasField("allowFlipV")) {
            if (*scale.getField("ranged")->getAsGroup()->getField("allowFlipV")->getAsBool())
                minScale.y *= -1;
        }        
    }

    const JsonGroup& pos = *data.getField("positioning")->getAsGroup();
    if (pos.hasField("fixedSpacing") || pos.hasField("rangedSpacing")) {
        sf::Vector2f minSpace, maxSpace;
        if (pos.hasField("rangedSpacing")) {
            minSpace.x = *pos.getField("rangedSpacing")->getAsGroup()->getField("minx")->getAsNumeric();
            minSpace.y = *pos.getField("rangedSpacing")->getAsGroup()->getField("miny")->getAsNumeric();
            maxSpace.x = *pos.getField("rangedSpacing")->getAsGroup()->getField("maxx")->getAsNumeric();
            maxSpace.y = *pos.getField("rangedSpacing")->getAsGroup()->getField("maxy")->getAsNumeric();
        }
        else {
            minSpace.x = *pos.getField("fixedSpacing")->getAsGroup()->getField("x")->getAsNumeric();
            minSpace.y = *pos.getField("fixedSpacing")->getAsGroup()->getField("y")->getAsNumeric();
            maxSpace = minSpace;
        }
        return BackgroundElementGenerator::Ptr(
            new SpacedElementGenerator(gfx, preserveAR, minScale, maxScale, minSpace, maxSpace)
        );
    }
    else if (pos.hasField("random")) {
        const float density = *pos.getField("random")->getAsGroup()->getField("density")->getAsNumeric();
        return BackgroundElementGenerator::Ptr(
            new RandomElementGenerator(gfx, density, preserveAR, minScale, maxScale)
        );
    }
    return nullptr;
}