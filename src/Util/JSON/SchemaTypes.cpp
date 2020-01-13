#include <Util/JSON/SchemaTypes.hpp>

#include <iostream>
#include <algorithm>
#include <unordered_set>

const SchemaValue SchemaValue::anyString(std::list<std::string>(0));
const SchemaValue SchemaValue::anyBool;
const SchemaValue SchemaValue::anyNumber({}, {});
const SchemaValue SchemaValue::negativeNumber({}, 0);
const SchemaValue SchemaValue::positiveNumber(0, {});

namespace {
std::ostream& error(const JsonSourceInfo& info) {
    std::cerr << "File " << info.filename << " line " << info.lineNumber << ": ";
    return std::cerr;
}
}

struct SchemaGroup::Field {
    const std::string name;
    const SchemaValue value;
    const bool required;

    Field(const std::string& name, const SchemaValue& value, bool required)
        : name(name), value(value), required(required) {}
};

SchemaGroup::SchemaGroup(bool overrideStrict, bool isStrict)
: overrideStrict(overrideStrict), isStrict(isStrict) {}

void SchemaGroup::addExpectedField(const std::string& name, const SchemaValue& value) {
    schema.push_back(std::make_shared<Field>(name, value, true));
}

void SchemaGroup::addOptionalField(const std::string& name, const SchemaValue& value) {
    schema.push_back(std::make_shared<Field>(name, value, false));
}

bool SchemaGroup::validate(const JsonGroup& group, bool strict) const {
    const bool beStrict = overrideStrict ? isStrict : strict;

    bool valid = true;
    std::vector<std::string> fields = group.getFields();
    std::unordered_set<std::string> expectedFields;
    std::unordered_set<std::string> optionalFields;

    const auto cb = [&expectedFields, &optionalFields] (const std::shared_ptr<Field>& ptr) {
        const Field& val = *ptr;
        if (val.required)
            expectedFields.insert(val.name);
        else
            optionalFields.insert(val.name);
    };
    std::for_each(schema.begin(), schema.end(), cb);

    for (auto fieldPtr : schema) {
        const Field& field = *fieldPtr;
        if (!group.hasField(field.name)) {
            if (optionalFields.find(field.name) == optionalFields.end()) {
                error(group.info()) << "JsonGroup is missing field: " << field.name << std::endl;
                valid = false;
            }
        }
        else {
            if (!field.value.validate(*group.getField(field.name), beStrict)) {
                error(group.getField(field.name)->info()) << "Field '" << field.name << "' failed to validate" << std::endl;
                valid = false;
            }
            auto iter = expectedFields.find(field.name);
            if (iter != expectedFields.end())
                expectedFields.erase(iter);
            fields.erase(std::find(fields.begin(), fields.end(), field.name));
        }
    }

    if (beStrict && fields.size() > 0) {
        error(group.info()) << "JsonGroup has extra fields: ";
        for (const std::string& field : fields)
            std::cerr << field << ", ";
        std::cerr << std::endl;
        valid = false;
    }

    return valid;
}

SchemaUnion::SchemaUnion(unsigned int nFields) : nRequiredFields(nFields) {}

void SchemaUnion::addFieldOption(const std::string& name, const SchemaValue& value) {
    fieldOptions.insert(std::make_pair(name, value));
}

bool SchemaUnion::validate(const JsonGroup& group, bool strict) const {
    bool valid = true;
    const std::vector<std::string> fields = group.getFields();

    if (fields.size() != nRequiredFields) {
        error(group.info()) << "Expected " << nRequiredFields << " got " << fields.size() << std::endl;
        valid = false;
    }

    int found = 0;
    for (unsigned int i = 0; i<fields.size(); ++i) {
        const auto iter = fieldOptions.find(fields[i]);
        if (iter == fieldOptions.end()) {
            error(group.info()) << "Unexpected field '" << fields[i] << "'\n";
            valid = false;
        }
        else {
            found += 1;
            if (!iter->second.validate(*group.getField(fields[i]), strict))
                valid = false;
        }
    }

    if (found != nRequiredFields) {
        error(group.info()) << nRequiredFields << " fields required from [";
        for (auto i = fieldOptions.begin(); i!=fieldOptions.end(); ++i)
            std::cerr << "'" << i->first << "', ";
        std::cerr << "]. " << found << " are present" << std::endl;
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

SchemaValue::SchemaValue(const SchemaUnion& unionGroup)
: type(JsonValue::Group)
, data(std::make_shared<TData>(unionGroup)) {}

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
            const SchemaGroup* group = std::get_if<SchemaGroup>(data.get());
            const SchemaUnion* uGrp = std::get_if<SchemaUnion>(data.get());
            if (val) {
                if (group)
                    return group->validate(*val, strict);
                else if (uGrp)
                    return uGrp->validate(*val, strict);
                else {
                    error(value.info()) << "JsonValue error: Schema object is incorrect type (expect group or union" << std::endl;
                    return false;
                }
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