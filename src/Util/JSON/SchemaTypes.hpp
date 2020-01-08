#ifndef SCHEMATYPES_HPP
#define SCHEMATYPES_HPP

#include <Util/JSON/JsonTypes.hpp>
#include <optional>
#include <list>

class SchemaGroup;
class SchemaValue;

/**
 * Helper struct for list types
 */
struct SchemaList {
    SchemaValue& listType;
    std::optional<int> minLen;
    std::optional<int> maxLen;
};

/**
 * Validates a group of values in json
 */
class SchemaGroup {
public:
    void addExpectedField(const std::string& name, SchemaValue& value);

    bool validate(const JsonGroup& data, bool strict) const;

private:
    std::vector<std::pair<std::string, SchemaValue&> > schema;
};

/**
 * Object to validate a json value against a schema
 */
class SchemaValue {
public:
    /**
     * Expects to be a boolean value
     */
    explicit SchemaValue();

    /**
     * Expect to be a List with the given type and length requirements
     */
    SchemaValue(const SchemaList& list);

    /**
     * Expect to be a group with the given structure
     */
    SchemaValue(const SchemaGroup& group);

    /**
     * Expect to be a numeric type with optional bounds
     */
    SchemaValue(std::optional<double> minVal, std::optional<double> maxVal);

    /**
     * Expect to be a string type, optionally one of a list of valid values
     */
    SchemaValue(const std::list<std::string>& values);

    /**
     * Performs validation on the given JsonValue
     */
    bool validate(const JsonValue& value, bool strict) const;

private:
    struct blank {};

    const JsonValue::Type type;
    const std::variant<
        SchemaList,
        SchemaGroup,
        std::pair<std::optional<double>, std::optional<double> >,
        std::list<std::string>,
        blank
    > data;
};

#endif