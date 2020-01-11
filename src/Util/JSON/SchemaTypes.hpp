#ifndef SCHEMATYPES_HPP
#define SCHEMATYPES_HPP

#include <Util/JSON/JsonTypes.hpp>
#include <memory>
#include <optional>
#include <list>

class SchemaGroup;
struct SchemaList;

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
    SchemaValue(std::optional<float> minVal, std::optional<float> maxVal);

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

    typedef std::variant<
        SchemaList,
        SchemaGroup,
        std::pair<std::optional<float>, std::optional<float> >,
        std::list<std::string>,
        blank
    > TData;

    const JsonValue::Type type;
    std::shared_ptr<const TData> data;
};

/**
 * Helper struct for list types
 */
struct SchemaList {
    SchemaValue listType;
    std::optional<int> minLen;
    std::optional<int> maxLen;
};

/**
 * Validates a group of values in json
 */
class SchemaGroup {
public:
    SchemaGroup(bool overrideStrict = false, bool isStrict = false);

    void addExpectedField(const std::string& name, const SchemaValue& value);
    void addOptionalField(const std::string& name, const SchemaValue& value);

    bool validate(const JsonGroup& data, bool strict) const;

private:
    struct Field;

    const bool overrideStrict;
    const bool isStrict;
    std::vector<std::shared_ptr<Field> > schema;
};

#endif