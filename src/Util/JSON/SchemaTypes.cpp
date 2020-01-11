#include <Util/JSON/SchemaTypes.hpp>

#include <iostream>
#include <algorithm>

namespace {
std::ostream& error(const JsonSourceInfo& info) {
    std::cerr << "File " << info.filename << " line " << info.lineNumber << ": ";
    return std::cerr;
}
}

void SchemaGroup::addExpectedField(const std::string& name, const SchemaValue& value) {
    schema.push_back(std::pair<std::string, SchemaValue>(name, value));
}

bool SchemaGroup::validate(const JsonGroup& group, bool strict) const {
    bool valid = true;
    std::vector<std::string> fields = group.getFields();
    std::vector<std::string> expectedFields;
    const auto cb = [&expectedFields] (const std::pair<std::string, SchemaValue>& val) {
        expectedFields.push_back(val.first);
    };
    std::for_each(schema.begin(), schema.end(), cb);

    for (auto field : schema) {
        if (!group.hasField(field.first)) {
            error(group.info()) << "JsonGroup is missing field: " << field.first << std::endl;
            valid = false;
        }
        else {
            if (!field.second.validate(*group.getField(field.first), strict))
                valid = false;
            expectedFields.erase(std::find(expectedFields.begin(), expectedFields.end(), field.first));
            fields.erase(std::find(fields.begin(), fields.end(), field.first));
        }
    }

    if (strict && fields.size() > 0) {
        error(group.info()) << "JsonGroup has extra fields: ";
        for (const std::string& field : fields)
            std::cerr << field << ", ";
        std::cerr << std::endl;
        valid = false;
    }

    return valid;
}

SchemaValue::SchemaValue()
: type(JsonValue::Bool)
, data(std::make_shared<TData>(blank())) {}

SchemaValue::SchemaValue(const SchemaList& listType)
: type(JsonValue::List)
, data(std::make_shared<TData>(listType)) {}

SchemaValue::SchemaValue(const SchemaGroup& group)
: type(JsonValue::Group)
, data(std::make_shared<TData>(group)) {}

SchemaValue::SchemaValue(std::optional<float> minVal, std::optional<float> maxVal)
: type(JsonValue::Numeric)
, data(std::make_shared<TData>(std::make_pair(minVal, maxVal))) {}

SchemaValue::SchemaValue(const std::list<std::string>& values)
: type(JsonValue::String)
, data(std::make_shared<TData>(values)) {}

bool SchemaValue::validate(const JsonValue& value, bool strict) const {
    if (type != value.getType()) {
        error(value.info()) << "Invalid JsonValue type: Expecting " << type << " got " << value.getType() << std::endl;
        return false;
    }

    switch (type) {
    case JsonValue::Numeric: {
            const float* val = value.getAsNumeric();
            if (val) {
                auto limits = *std::get_if<std::pair<std::optional<float>, std::optional<float> > >(data.get());
                if (*val < limits.first.value_or((*val) - 1)) {
                    error(value.info()) << "Numeric JsonValue is too low. Min: " << limits.first.value() << std::endl;
                    return false;
                }
                if (*val > limits.second.value_or((*val) + 1)) {
                    error(value.info()) << "Numeric JsonValue is too high. Max: " << limits.second.value() << std::endl;
                    return false;
                }
            }
            else {
                error(value.info()) << "JsonValue error: Type is Numeric but data not valid" << std::endl;
                return false;
            }
        }
        break;

    case JsonValue::String: {
            const std::string* val = value.getAsString();
            if (val) {
                auto values = *std::get_if<std::list<std::string> >(data.get());
                if (values.size() > 0) {
                    if (std::find(values.begin(), values.end(), *val) == values.end()) {
                        error(value.info()) << '"' << *val << "' is not a valid String value. Must be in [";
                        auto iter = values.begin();
                        std::cerr << '"' << *iter << '"';
                        for (; iter != values.end(); ++iter)
                            std::cerr << ", \"" << *iter << '"';
                        std::cerr << "]" << std::endl;
                        return false;
                    }
                }
            }
            else {
                error(value.info()) << "JsonValue error: Type is String but data is not valid" << std::endl;
                return false;
            }
        }
        break;

    case JsonValue::Group: {
            const JsonGroup* val = value.getAsGroup();
            if (val) {
                const SchemaGroup& schema = *std::get_if<SchemaGroup>(data.get());
                return schema.validate(*val, strict);
            }
            else {
                error(value.info()) << "JsonValue error: Type is Group but data is not valid" << std::endl;
                return false;
            }
        }
        break;

    case JsonValue::List: {
            const JsonList* val = value.getAsList();
            if (val) {
                bool valid = true;
                const SchemaList& schema = *std::get_if<SchemaList>(data.get());

                if (val->size() < schema.minLen.value_or(val->size() - 1)) {
                    error(value.info()) << "List size is too small: Min " << schema.minLen.value() << " actual " << val->size() << std::endl;
                }
                if (val->size() > schema.maxLen.value_or(val->size() + 1)) {
                    error(value.info()) << "List size is too big: Max " << schema.maxLen.value() << " actual " << val->size() << std::endl;
                }

                for (unsigned int i = 0; i<val->size(); ++i) {
                    if (!schema.listType.validate(val->at(i), strict))
                        valid = false;
                }
                return valid;
            }
            else {
                error(value.info()) << "JsonValue error: Type is List but data is invalid" << std::endl;
                return false; 
            }
        }
        break;

    case JsonValue::Bool:
        return true;

    default:
        error(value.info()) << "SchemaValue errpr: Invalid type " << type << std::endl;
        return false;
    }

    return true;
}