#include <Environment/Background.hpp>
#include <Environment/Backgrounds/ElementGeneratorFactory.hpp>
#include <Util/Schemas.hpp>
#include <iostream>

void Background::load(const JsonGroup& data) {
    if (!Schemas::backgroundSchema().validate(data, true)) {
        std::cerr << "Leaving background blank\n";
        return;
    }

    const JsonGroup& colorGroup = *data.getField("color")->getAsGroup();
    color.r = *colorGroup.getField("red")->getAsNumeric();
    color.g = *colorGroup.getField("green")->getAsNumeric();
    color.b = *colorGroup.getField("blue")->getAsNumeric();

    const JsonList& egens = *data.getField("elements")->getAsList();
    generators.reserve(egens.size());
    for (unsigned int i = 0; i<egens.size(); ++i) {
        BackgroundElementGenerator::Ptr gen = ElementGeneratorFactory::create(*egens[i].getAsGroup());
        if (gen)
            generators.push_back(gen);
    }
}

void Background::update(const sf::FloatRect& region) {
    for (unsigned int i = 0; i<generators.size(); ++i) {
        generators[i]->update(region);
    }
}

void Background::render(sf::RenderTarget& target) {
    target.clear(color);
    for (unsigned int i = 0; i<generators.size(); ++i) {
        generators[i]->render(target);
    }
}