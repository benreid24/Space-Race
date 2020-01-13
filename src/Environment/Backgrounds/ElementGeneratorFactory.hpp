#ifndef ELEMENTGENERATORFACTORY_HPP
#define ELEMENTGENERATORFACTORY_HPP

#include <Environment/Backgrounds/BackgroundElementGenerator.hpp>
#include <Util/JsonFile.hpp> //TODO - refactor json and all file layouts and includes

struct ElementGeneratorFactory {
    static BackgroundElementGenerator::Ptr create(const JsonGroup& data);
};

#endif