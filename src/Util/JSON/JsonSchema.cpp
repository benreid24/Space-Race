#include <Util/JSON/JsonSchema.hpp>
#include <Util/JsonFile.hpp>

namespace {

JsonSchema createEnvironmentSchema() {
    // Common Types
    SchemaValue boolValue;
    SchemaValue anyString(std::list<std::string>(0));
    SchemaValue anyNumber({}, {});
    SchemaValue positiveNumber(0, {});
    SchemaValue colorNumber(0, 255);

    // Entity List
    SchemaGroup entityGroup;
    entityGroup.addExpectedField("name", anyString);
    entityGroup.addExpectedField("gfx", anyString);
    entityGroup.addExpectedField("x", anyNumber);
    entityGroup.addExpectedField("y", anyNumber);
    entityGroup.addExpectedField("vx", anyNumber);
    entityGroup.addExpectedField("vy", anyNumber);
    entityGroup.addExpectedField("mass", positiveNumber);
    entityGroup.addExpectedField("canMove", boolValue);
    entityGroup.addExpectedField("hasGravity", boolValue);
    entityGroup.addOptionalField("gravityRange", positiveNumber);
    SchemaValue entityValue(entityGroup);
    SchemaList entityList = {entityValue}; // no min/max size
    SchemaValue entityListValue(entityList);

    // Color
    SchemaGroup colorGroup;
    colorGroup.addExpectedField("red", colorNumber);
    colorGroup.addExpectedField("green", colorNumber);
    colorGroup.addExpectedField("blue", colorNumber);
    SchemaValue colorValue(colorGroup);

    // Background
    SchemaGroup backgroundGroup;
    backgroundGroup.addExpectedField("color", colorValue);
    // TODO - implement "one of" expectation. Separate struct w/ fields. adds to group
    SchemaValue backgroundValue(backgroundGroup);

    // Player Spawn
    SchemaGroup spawnGroup;
    spawnGroup.addExpectedField("x", anyNumber);
    spawnGroup.addExpectedField("y", anyNumber);
    SchemaValue spawnValue(spawnGroup);

    // Victory zone
    SchemaGroup winZoneGroup;
    winZoneGroup.addExpectedField("top", positiveNumber);
    winZoneGroup.addExpectedField("left", positiveNumber);
    winZoneGroup.addExpectedField("width", positiveNumber);
    winZoneGroup.addExpectedField("height", positiveNumber);
    SchemaValue winGroupValue(winZoneGroup);

    // Main Schema
    SchemaGroup mainGroup;
    mainGroup.addExpectedField("name", anyString);
    mainGroup.addExpectedField("width", positiveNumber);
    mainGroup.addExpectedField("height", positiveNumber);
    mainGroup.addExpectedField("background", backgroundValue);
    mainGroup.addExpectedField("winZone", winGroupValue);
    mainGroup.addExpectedField("playerSpawn", spawnValue);
    mainGroup.addExpectedField("entities", entityListValue);
    
    return JsonSchema(mainGroup);
}

}

JsonSchema::JsonSchema(const SchemaGroup& root)
: root(root) {}

bool JsonSchema::validate(const JsonFile& file, bool strict) const {
    return root.validate(file.getRoot(), strict);
}

const JsonSchema& JsonSchema::environmentFileSchema() {
    static const JsonSchema schema = createEnvironmentSchema();
    return schema;
}