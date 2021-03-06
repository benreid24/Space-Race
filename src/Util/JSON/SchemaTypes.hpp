#ifndef SCHEMATYPES_HPP
#define SCHEMATYPES_HPP

#include <Util/JSON/JsonTypes.hpp>
#include <memory>
#include <optional>
#include <list>

class SchemaGroup;
struct SchemaList;
class SchemaUnion;

/**
 * Object to validate a json value against a schema
 */
class SchemaValue {
public:
    static const SchemaValue anyString;
    static const SchemaValue anyNumber;
    static const SchemaValue positiveNumber;
    static const SchemaValue negativeNumber;
    static const SchemaValue anyBool;

    /**
     * Expect to be a List with the given type and length requirements
     */
    explicit SchemaValue(const SchemaList& list);

    /**
     * Expect to be a group with the given structure
     */
    explicit SchemaValue(const SchemaGroup& group);

    /**
     * Expect to be a union of specific values
     */
    explicit SchemaValue(const SchemaUnion& unionGroup);

    /**
     * Expect to be a numeric type with optional bounds
     */
    explicit SchemaValue(std::optional<float> minVal, std::optional<float> maxVal);

    /**
     * Expect to be a string type, optionally one of a list of valid values
     */
    explicit SchemaValue(const std::list<std::string>& values);

    /**
     * Performs validation on the given JsonValue
     */
    bool validate(const JsonValue& value, bool strict) const;

    JsonValue::Type getType() const { return type; }

private:
    /**
     * Expects to be a boolean value
     */
    explicit SchemaValue();

    struct blank {};

    typedef std::variant<
        SchemaList,
        SchemaGroup,
        SchemaUnion,
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
    const SchemaValue listType;
    const std::optional<int> minLen;
    const std::optional<int> maxLen;

    explicit SchemaList(const SchemaValue& t) : listType(t) {}
    explicit SchemaList(const SchemaValue& t, int min) : listType(t), minLen(min) {}
    explicit SchemaList(const SchemaValue& t, int min, int max) : listType(t), minLen(min), maxLen(max) {}
};

/**
 * Validates a group of values in json
 */
class SchemaGroup {
public:
    explicit SchemaGroup(bool overrideStrict = false, bool isStrict = false);

    void addExpectedField(const std::string& name, const SchemaValue& value);
    void addOptionalField(const std::string& name, const SchemaValue& value);

    bool validate(const JsonGroup& data, bool strict) const;

private:
    struct Field;

    const bool overrideStrict;
    const bool isStrict;
    std::vector<std::shared_ptr<Field> > schema;
};

/**
 * Specialized group where N field(s) from a list of options must be selected
 */
class SchemaUnion {
public:
    explicit SchemaUnion(unsigned int nFieldsRequired = 1);

    void addFieldOption(const std::string& name, const SchemaValue& value);

    bool validate(const JsonGroup& group, bool strict) const;

private:
    const unsigned int nRequiredFields;
    std::map<std::string, SchemaValue> fieldOptions;
};

#endif