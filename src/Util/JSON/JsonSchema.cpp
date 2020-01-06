#include <Util/JSON/JsonSchema.hpp>
#include <Util/JsonFile.hpp>

JsonSchema::JsonSchema(const SchemaGroup& root)
: root(root) {}

bool JsonSchema::validate(const JsonFile& file, bool strict) {
    return root.validate(file.getRoot(), strict);
}

//TODO - schemas