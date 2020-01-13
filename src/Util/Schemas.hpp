#ifndef SCHEMAS_HPP
#define SCHEMAS_HPP

#include <Util/JSON/JsonSchema.hpp>

struct Schemas {
    /**
     * Returns the schema for the Environment file
     */
    static const JsonSchema& environmentFileSchema();

    /**
     * Returns the schema for an Entity
     */
    static const JsonSchema& entitySchema();

    /**
     * Returns the schema for a background
     */
    static const JsonSchema& backgroundSchema();
};

#endif