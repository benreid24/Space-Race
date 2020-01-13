#ifndef JSONSCHEMA_HPP
#define JSONSCHEMA_HPP

#include <Util/JSON/SchemaTypes.hpp>
#include <Util/JsonFile.hpp>

/**
 * Validates a JsonFile object by defining a schema for it
 */
class JsonSchema {
public:
    /**
     * Defines a schema with the given root group object
     */
    JsonSchema(const SchemaGroup& root) : root(root) {}

    /**
     * Returns the root object
     */
    const SchemaGroup& getRoot() const { return root; }

    /**
     * Validates the given JsonFile
     * 
     * \param file The data to validate
     * \param strict Whether or not extra fields are considered errors
     */
    bool validate(const JsonFile& file, bool strict) const { return root.validate(file.getRoot(), strict); }

    /**
     * Validates the given JsonGroup
     * 
     * \param data The data to validate
     * \param strict Whether or not extra fields are considered errors
     */
    bool validate(const JsonGroup& data, bool strict) const { return root.validate(data, strict); }

private:
    const SchemaGroup root;
};

#endif