#include <Util/JSON/JsonTypes.hpp>

#include <Util/JSON/JsonLoader.hpp>
#include <iostream>

JsonValue::JsonValue(const std::string& value)
: type(String), data(value) {}

JsonValue::JsonValue(double value)
: type(Numeric), data(value) {}

JsonValue::JsonValue(const JsonList& value)
: type(List), data(value) {}

JsonValue::JsonValue(const JsonGroup& value)
: type(Group), data(value) {}

JsonValue::Type JsonValue::getType() const {
    return type;
}

const std::string* JsonValue::getAsString() const {
    return std::get_if<std::string>(&data);
}

const double* JsonValue::getAsNumeric() const {
    return std::get_if<double>(&data);
}

const JsonList* JsonValue::getAsList() const {
    return std::get_if<JsonList>(&data);
}

const JsonGroup* JsonValue::getAsGroup() const {
    return std::get_if<JsonGroup>(&data);
}

void JsonGroup::addField(const std::string& name, const JsonField& field) {
    if (fields.find(name) != fields.end()) {
        std::cerr << "Warning: Overwriting field \"" << name << "\" in JsonGroup" << std::endl;
    }
    fields.insert(std::make_pair(name, field));
    fieldNames.push_back(name);
}

bool JsonGroup::hasField(const std::string& name) const {
    return fields.find(name) != fields.end();
}

const JsonField* JsonGroup::getField(const std::string& name) const {
    auto iter = fields.find(name);
    if (iter != fields.end())
        return &iter->second;
    return nullptr;
}

const std::vector<std::string> JsonGroup::getFields() const {
    return fieldNames;
}

//                  OUTPUT

std::ostream& operator<<(std::ostream& stream, const JsonValue::Type& type) {
    switch (type) {
        case JsonValue::String:
            stream << "String";
            break;
        case JsonValue::Numeric:
            stream << "Numeric";
            break;
        case JsonValue::List:
            stream << "List";
            break;
        case JsonValue::Group:
            stream << "Group";
            break;
        default:
            stream << "UNKNOWN";
            break;
    }
    return stream;
}

void JsonListUtil::print(const JsonList& list, std::ostream& stream, int ilvl) {
    stream << "[";
    if (list.size() > 0)
        stream << "\n";
    for (unsigned int i = 0; i<list.size(); ++i) {
        stream << std::string(ilvl, ' ');
        list[i].print(stream, ilvl+4);
    }
    stream << std::string(ilvl-4, ' ') << "],\n";
}

void JsonField::print(std::ostream& stream, int ilvl) const {
    stream << '"' << getName() << "\": ";
    getValue().print(stream, ilvl);
}

void JsonGroup::print(std::ostream& stream, int ilvl) const {
    stream << "{";
    if (fields.size() > 0)
        stream << '\n';
    for (auto i = fields.begin(); i!=fields.end(); ++i) {
        stream << std::string(ilvl, ' ');
        i->second.print(stream, ilvl+4);
    }
    stream << std::string(ilvl-4, ' ') << "}," << std::endl;
}

void JsonValue::print(std::ostream& stream, int ilvl) const {
    switch (getType()) {
        case Numeric:
            stream << *getAsNumeric() << ",\n";
            break;
        case String:
            stream << '"' << *getAsString() << "\",\n";
            break;
        case Group:
            getAsGroup()->print(stream, ilvl);
            break;
        case List:
            JsonListUtil::print(*getAsList(), stream, ilvl);
            break;
    }
}

//                     LOADING

JsonGroup JsonGroup::load(JsonLoader& input) {
    static const JsonGroup errVal;
    if (!input.isValid())
        return errVal;

    const JsonSourceInfo info = {input.getFilename(), input.currentLine()};

    if (input.peekNextSymbol() != '{') {
        input.error() << "Unexpected symbol '" << input.peekNextSymbol() << " expected '{'" << std::endl;
        return errVal;
    }
    input.skipSymbol();

    JsonGroup ret;
    ret.source = info;

    while (input.peekNextSymbol() == '"') {
        JsonField field = JsonField::load(input);
        if (!input.isValid())
            return errVal;
        ret.addField(field.getName(), field);
        if (input.peekNextSymbol() == ',')
            input.skipSymbol(); //trailing comma ok
    }

    if (input.peekNextSymbol() != '}') {
        input.error() << "Expected '}' got '" << input.peekNextSymbol() << '\'' << std::endl;
        return errVal;
    }
    input.skipSymbol();

    return ret;
}

JsonList JsonListUtil::load(JsonLoader& input) {
    static const JsonList errVal;
    if (!input.isValid())
        return errVal;

    if (input.peekNextSymbol() != '[') {
        input.error() << "Expected '[' got '" << input.peekNextSymbol() << '\'' << std::endl;
        return errVal;
    }
    input.skipSymbol();

    JsonList ret;
    while (input.peekNextSymbol() != ']') {
        if (!input.isValid()) {
            input.error() << "Unexpected end of file" << std::endl;
            return errVal;
        }

        JsonValue value = JsonValue::load(input);
        if (!input.isValid())
            return errVal;
        ret.push_back(value);

        if (ret.size() > 1) {
            if (ret[ret.size()-1].getType() != ret[ret.size()-2].getType()) {
                input.error() << "Types in list must all be the same" << std::endl;
                return errVal;
            }
        }

        if (input.peekNextSymbol() == ',')
            input.skipSymbol(); //trailing comma ok
    }
    input.skipSymbol();

    return ret;
}

JsonValue JsonValue::load(JsonLoader& input) {
    static const JsonValue errVal;
    if (!input.isValid())
        return errVal;

    const JsonSourceInfo info = {input.getFilename(), input.currentLine()};

    if (input.peekNextSymbol() == '"') {
        JsonValue value(input.loadString());
        value.source = info;
        return value;
    }
    if (input.isNumeric(input.peekNextSymbol())) {
        JsonValue value(input.loadNumeric());
        value.source = info;
        return value;
    }
    if (input.peekNextSymbol() == '{') {
        const JsonGroup group = JsonGroup::load(input);
        if (!input.isValid())
            return errVal;
        JsonValue value(group);
        value.source = info;
        return value;
    }
    if (input.peekNextSymbol() == '[') {
        const JsonList list = JsonListUtil::load(input);
        if (!input.isValid())
            return errVal;
        JsonValue value(list);
        value.source = info;
        return value;
    }

    input.error() << "Unexpected symbol '" << input.peekNextSymbol() << "' expected Value" << std::endl;
    return errVal;
}

JsonField JsonField::load(JsonLoader& input) {
    static const JsonField errVal;
    if (!input.isValid())
        return errVal;

    const JsonSourceInfo info = {input.getFilename(), input.currentLine()};

    if (input.peekNextSymbol() != '"') {
        input.error() << "Expected '\"'" << std::endl;
        return errVal;
    }
    const std::string name = input.loadString();

    if (input.peekNextSymbol() != ':') {
        input.error() << "Expecting ':'" << std::endl;
    }
    input.skipSymbol();
    if (!input.isValid())
        return errVal;
        
    const JsonValue value = JsonValue::load(input);

    JsonField ret(name, value);
    ret.source = info;
    return ret;
}