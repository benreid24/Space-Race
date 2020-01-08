#include <Util/JSON/JsonSchema.hpp>
#include <Util/JsonFile.hpp>

JsonSchema::JsonSchema(const SchemaGroup& root)
: root(root) {}

bool JsonSchema::validate(const JsonFile& file, bool strict) const {
    return root.validate(file.getRoot(), strict);
}

const JsonSchema& JsonSchema::environmentFileSchema() {
    // Common Types
    static SchemaValue boolValue;
    static SchemaValue anyString(std::list<std::string>(0));
    static SchemaValue anyNumber({}, {});
    static SchemaValue positiveNumber(0, {});

    // Entity List
    static SchemaGroup entityGroup;
    static bool eGrpInit = false;
    if (!eGrpInit) {
        eGrpInit = true;
        entityGroup.addExpectedField("name", anyString);
        entityGroup.addExpectedField("gfx", anyString);
        entityGroup.addExpectedField("x", anyNumber);
        entityGroup.addExpectedField("y", anyNumber);
        entityGroup.addExpectedField("vx", anyNumber);
        entityGroup.addExpectedField("vy", anyNumber);
        entityGroup.addExpectedField("mass", positiveNumber);
        entityGroup.addExpectedField("canMove", boolValue);
        entityGroup.addExpectedField("hasGravity", boolValue);
        entityGroup.addExpectedField("gravityRange", positiveNumber);
    }
    static SchemaValue entityValue(entityGroup);
    static SchemaList entityList = {entityValue}; // no min/max size
    static SchemaValue entityListValue(entityList);

    // Player Spawn
    static SchemaGroup spawnGroup;
    static bool spGrpInit = false;
    if (!spGrpInit) {
        spGrpInit = true;
        spawnGroup.addExpectedField("x", anyNumber);
        spawnGroup.addExpectedField("y", anyNumber);
    }
    static SchemaValue spawnValue(spawnGroup);

    // Victory zone
    static SchemaGroup winZoneGroup;
    static bool wzGrpInit = false;
    if (!wzGrpInit) {
        wzGrpInit = true;
        winZoneGroup.addExpectedField("top", positiveNumber);
        winZoneGroup.addExpectedField("right", positiveNumber);
        winZoneGroup.addExpectedField("bottom", positiveNumber);
        winZoneGroup.addExpectedField("left", positiveNumber);
    }
    static SchemaValue winGroupValue(winZoneGroup);

    // Main Schema
    static SchemaGroup mainGroup;
    static bool schemaInit = false;
    if (!schemaInit) {
        schemaInit = true;
        mainGroup.addExpectedField("name", anyString);
        mainGroup.addExpectedField("width", positiveNumber);
        mainGroup.addExpectedField("height", positiveNumber);
        mainGroup.addExpectedField("winZone", winGroupValue);
        mainGroup.addExpectedField("playerSpawn", spawnValue);
        mainGroup.addExpectedField("entities", entityListValue);
    }

    static JsonSchema schema(mainGroup);
    return schema;
}