#ifndef JSONSCHEMA_HPP
#define JSONSCHEMA_HPP

#include <Util/JSON/SchemaTypes.hpp>

class JsonFile;

/**
 * Validates a JsonFile object by defining a schema for it
 */
class JsonSchema {
public:
    /**
     * Defines a schema with the given root group object
     */
    JsonSchema(const SchemaGroup& root);

    /**
     * Validates the given JsonFile
     * 
     * \param file The data to validate
     * \param strict Whether or not extra fields are considered errors
     */
    bool validate(const JsonFile& file, bool strict);

    /**
     * Returns the schema for the Environment file
     */
    static const JsonSchema& environmentFileSchema();

private:
    const SchemaGroup root;
};

#endif