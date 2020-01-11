#include <Util/JSON/JsonSchema.hpp>
#include <Util/JsonFile.hpp>

#include <iostream>

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
    SchemaList entityList(entityValue);
    SchemaValue entityListValue(entityList);

    // Color
    SchemaGroup colorGroup;
    colorGroup.addExpectedField("red", colorNumber);
    colorGroup.addExpectedField("green", colorNumber);
    colorGroup.addExpectedField("blue", colorNumber);
    SchemaValue colorValue(colorGroup);

    // Generic range
    SchemaGroup rangeGroup;
    rangeGroup.addExpectedField("minx", positiveNumber);
    rangeGroup.addExpectedField("miny", positiveNumber);
    rangeGroup.addExpectedField("maxx", positiveNumber);
    rangeGroup.addExpectedField("maxy", positiveNumber);
    SchemaValue rangeValue(rangeGroup);

    // Image scale
    SchemaGroup fixedScale;
    fixedScale.addExpectedField("x", anyNumber);
    fixedScale.addExpectedField("y", anyNumber);
    SchemaUnion imageScale;
    imageScale.addFieldOption("fixed", SchemaValue(fixedScale));
    imageScale.addFieldOption("ranged", rangeValue);
    SchemaValue imageScaleValue(imageScale);

    // Image positioning
    SchemaGroup randomPosGroup;
    randomPosGroup.addExpectedField("density", SchemaValue(0, 1));
    SchemaUnion imagePosUnion;
    imagePosUnion.addFieldOption("random", SchemaValue(randomPosGroup));
    imagePosUnion.addFieldOption("fixedSpacing", SchemaValue(fixedScale));
    imagePosUnion.addFieldOption("rangedSpacing", rangeValue);
    SchemaValue imagePosValue(imagePosUnion);

    // Image list
    SchemaGroup imageGroup;
    imageGroup.addExpectedField("file", anyString);
    imageGroup.addExpectedField("positioning", imagePosValue);
    imageGroup.addExpectedField("scale", imageScaleValue);
    SchemaValue imageValue(imageGroup);
    SchemaList imageList(imageValue);
    SchemaValue imageListValue(imageList);

    // Background
    SchemaGroup backgroundGroup;
    backgroundGroup.addExpectedField("color", colorValue);
    backgroundGroup.addExpectedField("images", imageListValue);
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