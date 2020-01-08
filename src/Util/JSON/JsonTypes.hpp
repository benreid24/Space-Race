#ifndef JSONTYPES_HPP
#define JSONTYPES_HPP

#include <vector>
#include <map>
#include <string>
#include <variant>
#include <ostream>

class JsonField;
class JsonValue;
class JsonGroup;
class JsonLoader;

/**
 * Helper struct for json source info
 */
struct JsonSourceInfo {
    std::string filename;
    int lineNumber;
};

std::ostream& operator<<(std::ostream& stream, const JsonSourceInfo& info);

/**
 * List of JsonValue objects
 */
typedef std::vector<JsonValue> JsonList;
struct JsonListUtil {
    static JsonList load(JsonLoader& input);
    static void print(const JsonList& list, std::ostream& stream, int indentLevel = 4);
};

/**
 * Represents a collection of named JsonField objects
 */
class JsonGroup {
public:
    JsonGroup() {}

    void addField(const std::string& name, const JsonField& field);
    bool hasField(const std::string& name) const;
    const JsonField* getField(const std::string& name) const;
    const std::vector<std::string> getFields() const;
    
    const JsonSourceInfo& info() const { return source; }

    void print(std::ostream& stream, int indentLevel) const;
    static JsonGroup load(JsonLoader& input);

private:
    std::vector<std::string> fieldNames;
    std::map<std::string, JsonField> fields;
    JsonSourceInfo source;
};

/**
 * Represents a Value in json. Can be numeric, string, list of values, or group of fields
 */
class JsonValue {
public:
    enum Type {
        Bool,
        String,
        Numeric,
        Group,
        List,
        Unknown
    };

    JsonValue() : type(Unknown) {}
    JsonValue(bool value);
    JsonValue(double value);
    JsonValue(const std::string& value);
    JsonValue(const JsonList& value);
    JsonValue(const JsonGroup& value);

    Type getType() const;
    const bool* getAsBool() const;
    const std::string* getAsString() const;
    const double* getAsNumeric() const;
    const JsonList* getAsList() const;
    const JsonGroup* getAsGroup() const;

    const JsonSourceInfo& info() const { return source; }

    static JsonValue load(JsonLoader& input);
    void print(std::ostream& stream, int indentLevel = 4) const;

private:
    const Type type;
    const std::variant<bool, std::string, double, JsonGroup, JsonList> data;
    JsonSourceInfo source;
};

std::ostream& operator<<(std::ostream& stream, const JsonValue::Type& type);

/**
 * Represents a named json field. Has corresponding JsonValue
 * */
class JsonField {
public:
    JsonField() {}
    JsonField(const std::string& name, const JsonValue& value)
        : name(name), value(value) {}

    const std::string& getName() const { return name; }
    const JsonValue& getValue() const { return value; }

    const JsonSourceInfo& info() const { return source; }

    void print(std::ostream& stream, int indentLevel = 4) const;
    static JsonField load(JsonLoader& input);

private:
    JsonSourceInfo source;
    const std::string name;
    const JsonValue value;
};

#endif