#include <Util/Schemas.hpp>

namespace {

JsonSchema createBackgroundSchema() {
    // Generic range
    SchemaGroup rangeGroup;
    rangeGroup.addOptionalField("preserveAR", SchemaValue::anyBool);
    rangeGroup.addOptionalField("allowHFlip", SchemaValue::anyBool);
    rangeGroup.addOptionalField("allowVFlip", SchemaValue::anyBool);
    rangeGroup.addExpectedField("minx", SchemaValue::positiveNumber);
    rangeGroup.addExpectedField("miny", SchemaValue::positiveNumber);
    rangeGroup.addExpectedField("maxx", SchemaValue::positiveNumber);
    rangeGroup.addExpectedField("maxy", SchemaValue::positiveNumber);
    SchemaValue rangeValue(rangeGroup);

    // Image scale
    SchemaGroup fixedScale;
    fixedScale.addExpectedField("x", SchemaValue::anyNumber);
    fixedScale.addExpectedField("y", SchemaValue::anyNumber);
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

    // Image
    SchemaGroup imageGroup;
    imageGroup.addExpectedField("file", SchemaValue::anyString);
    imageGroup.addExpectedField("positioning", imagePosValue);
    imageGroup.addExpectedField("scale", imageScaleValue);
    SchemaValue imageValue(imageGroup);

    // Image list
    SchemaList imageList(imageValue);
    SchemaValue imageListValue(imageList);

    // Color
    SchemaValue colorNumber(0, 255);
    SchemaGroup colorGroup;
    colorGroup.addExpectedField("red", colorNumber);
    colorGroup.addExpectedField("green", colorNumber);
    colorGroup.addExpectedField("blue", colorNumber);
    SchemaValue colorValue(colorGroup);

    // Base group
    SchemaGroup backgroundGroup;
    backgroundGroup.addExpectedField("color", colorValue);
    backgroundGroup.addExpectedField("elements", imageListValue);
    
    return JsonSchema(backgroundGroup);
}

JsonSchema createEntitySchema() {
    SchemaGroup entityGroup;
    entityGroup.addExpectedField("name", SchemaValue::anyString);
    entityGroup.addExpectedField("gfx", SchemaValue::anyString);
    entityGroup.addExpectedField("x", SchemaValue::anyNumber);
    entityGroup.addExpectedField("y", SchemaValue::anyNumber);
    entityGroup.addExpectedField("vx", SchemaValue::anyNumber);
    entityGroup.addExpectedField("vy", SchemaValue::anyNumber);
    entityGroup.addExpectedField("mass", SchemaValue::positiveNumber);
    entityGroup.addExpectedField("canMove", SchemaValue::anyBool);
    entityGroup.addExpectedField("hasGravity", SchemaValue::anyBool);
    entityGroup.addOptionalField("gravityRange", SchemaValue::positiveNumber);
    
    return JsonSchema(entityGroup);
}

JsonSchema createEnvironmentSchema() {
    // Entity List
    SchemaList entityList(SchemaValue(createEntitySchema().getRoot()));
    SchemaValue entityListValue(entityList);

    // Background 
    SchemaValue backgroundValue(createBackgroundSchema().getRoot());

    // Player Spawn
    SchemaGroup spawnGroup;
    spawnGroup.addExpectedField("x", SchemaValue::anyNumber);
    spawnGroup.addExpectedField("y", SchemaValue::anyNumber);
    SchemaValue spawnValue(spawnGroup);

    // Victory zone
    SchemaGroup winZoneGroup;
    winZoneGroup.addExpectedField("top", SchemaValue::positiveNumber);
    winZoneGroup.addExpectedField("left", SchemaValue::positiveNumber);
    winZoneGroup.addExpectedField("width", SchemaValue::positiveNumber);
    winZoneGroup.addExpectedField("height", SchemaValue::positiveNumber);
    SchemaValue winGroupValue(winZoneGroup);

    // Main Schema
    SchemaGroup mainGroup;
    mainGroup.addExpectedField("name", SchemaValue::anyString);
    mainGroup.addExpectedField("width", SchemaValue::positiveNumber);
    mainGroup.addExpectedField("height", SchemaValue::positiveNumber);
    mainGroup.addExpectedField("background", backgroundValue);
    mainGroup.addExpectedField("winZone", winGroupValue);
    mainGroup.addExpectedField("playerSpawn", spawnValue);
    mainGroup.addExpectedField("entities", entityListValue);
    
    return JsonSchema(mainGroup);
}

} // namespace

const JsonSchema& Schemas::environmentFileSchema() {
    static const JsonSchema schema = createEnvironmentSchema();
    return schema;
}

const JsonSchema& Schemas::backgroundSchema() {
    static const JsonSchema schema = createBackgroundSchema();
    return schema;
}

const JsonSchema& Schemas::entitySchema() {
    static const JsonSchema schema = createEntitySchema();
    return schema;
}
