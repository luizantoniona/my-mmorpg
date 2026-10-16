#include <gtest/gtest.h>

#include <filesystem>
#include <optional>
#include <string>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Object/ObjectFactory.h>

namespace {

class ObjectFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_objectfactory";
        std::filesystem::create_directories( _tempDir / "TestMap" );

        Json::Value configJson;
        configJson[ "ActiveFolder" ] = "TestMap";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "Config.json" ).string(), configJson );

        Json::Value mapJson;
        mapJson[ "Catalogs" ][ "Tiles" ] = "Tile.json";
        mapJson[ "Catalogs" ][ "Objects" ] = "Object.json";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Map.json" ).string(), mapJson );
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    void writeObjectsJson( const Json::Value& objectsArray ) const {
        Json::Value objectJson;
        objectJson[ "Objects" ] = objectsArray;
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Object.json" ).string(), objectJson );
    }

    QString configPath() const {
        return QString::fromStdString( _tempDir.string() ) + "/";
    }

    std::filesystem::path _tempDir;
};

class InteractionCase {
public:
    InteractionCase( std::string name, bool present, std::string interaction, int containerCapacity, bool expectedPresent, Engine::ObjectInteractionEnum expectedType, uint32_t expectedCapacity ) :
        name( std::move( name ) ),
        present( present ),
        interaction( std::move( interaction ) ),
        containerCapacity( containerCapacity ),
        expectedPresent( expectedPresent ),
        expectedType( expectedType ),
        expectedCapacity( expectedCapacity ) {
    }

    std::string name;
    bool present;
    std::string interaction;
    int containerCapacity;
    bool expectedPresent;
    Engine::ObjectInteractionEnum expectedType;
    uint32_t expectedCapacity;
};

class FootprintCase {
public:
    FootprintCase( std::string name, bool present, int width, int height, int expectedWidth, int expectedHeight ) :
        name( std::move( name ) ),
        present( present ),
        width( width ),
        height( height ),
        expectedWidth( expectedWidth ),
        expectedHeight( expectedHeight ) {
    }

    std::string name;
    bool present;
    int width;
    int height;
    int expectedWidth;
    int expectedHeight;
};

class ObjectWrongTypeCase {
public:
    ObjectWrongTypeCase( std::string name, std::string parent, std::string key, Json::Value value ) :
        name( std::move( name ) ),
        parent( std::move( parent ) ),
        key( std::move( key ) ),
        value( std::move( value ) ) {
    }

    std::string name;
    std::string parent;
    std::string key;
    Json::Value value;
};

class ObjectFactoryWrongTypeTest : public ObjectFactoryTest, public ::testing::WithParamInterface<ObjectWrongTypeCase> {};
class ObjectFactoryInteractionTest : public ObjectFactoryTest, public ::testing::WithParamInterface<InteractionCase> {};
class ObjectFactoryFootprintTest : public ObjectFactoryTest, public ::testing::WithParamInterface<FootprintCase> {};

} // namespace

TEST_F( ObjectFactoryTest, CreateObjectCatalog_ParsesFields ) {
    Json::Value object;
    object[ "Type" ] = 1;
    object[ "Name" ] = "StoneWall";
    object[ "TextureFile" ] = "Textures/Objects/stone_wall_01";
    Json::Value footprint;
    footprint[ "Width" ] = 2;
    footprint[ "Height" ] = 3;
    object[ "Footprint" ] = footprint;

    Json::Value objects( Json::arrayValue );
    objects.append( object );
    writeObjectsJson( objects );

    Engine::ObjectCatalog catalog;
    Engine::ObjectFactory::createObjectCatalog( configPath(), catalog );

    ASSERT_NE( catalog.object( 1 ), nullptr );
    EXPECT_EQ( catalog.object( 1 )->name(), "StoneWall" );
    EXPECT_EQ( catalog.object( 1 )->textureFile(), "Textures/Objects/stone_wall_01" );
    EXPECT_EQ( catalog.object( 1 )->size().width(), 2 );
    EXPECT_EQ( catalog.object( 1 )->size().height(), 3 );
}

TEST_F( ObjectFactoryTest, CreateObjectCatalog_SameNameDifferentTextureFile_BothKept ) {
    Json::Value first;
    first[ "Type" ] = 1;
    first[ "Name" ] = "Tree";
    first[ "TextureFile" ] = "Textures/Objects/tree_01";

    Json::Value second;
    second[ "Type" ] = 2;
    second[ "Name" ] = "Tree";
    second[ "TextureFile" ] = "Textures/Objects/tree_02";

    Json::Value objects( Json::arrayValue );
    objects.append( first );
    objects.append( second );
    writeObjectsJson( objects );

    Engine::ObjectCatalog catalog;
    Engine::ObjectFactory::createObjectCatalog( configPath(), catalog );

    ASSERT_NE( catalog.object( 1 ), nullptr );
    ASSERT_NE( catalog.object( 2 ), nullptr );
    EXPECT_EQ( catalog.object( 1 )->name(), catalog.object( 2 )->name() );
    EXPECT_NE( catalog.object( 1 )->textureFile(), catalog.object( 2 )->textureFile() );
}

TEST_F( ObjectFactoryTest, SaveObjectCatalog_ThenCreateObjectCatalog_RoundTripsFields ) {
    Engine::ObjectCatalog original;
    Engine::ObjectModel object;
    object.setType( 5 );
    object.setName( "WoodenCrate" );
    object.setTextureFile( "Textures/Objects/wooden_crate_01" );
    Engine::ObjectSizeModel size;
    size.setWidth( 2 );
    size.setHeight( 1 );
    object.setSize( size );
    original.addObject( object );

    Engine::ObjectFactory::saveObjectCatalog( configPath(), original );

    Engine::ObjectCatalog reloaded;
    Engine::ObjectFactory::createObjectCatalog( configPath(), reloaded );

    ASSERT_NE( reloaded.object( 5 ), nullptr );
    EXPECT_EQ( reloaded.object( 5 )->name(), "WoodenCrate" );
    EXPECT_EQ( reloaded.object( 5 )->textureFile(), "Textures/Objects/wooden_crate_01" );
    EXPECT_EQ( reloaded.object( 5 )->size().width(), 2 );
    EXPECT_EQ( reloaded.object( 5 )->size().height(), 1 );
}

TEST_F( ObjectFactoryTest, CreateObjectCatalog_MissingIsWalkable_DefaultsToWalkable ) {
    Json::Value object;
    object[ "Type" ] = 1;
    object[ "Name" ] = "StoneWall";
    object[ "TextureFile" ] = "Textures/Objects/stone_wall_01";

    Json::Value objects( Json::arrayValue );
    objects.append( object );
    writeObjectsJson( objects );

    Engine::ObjectCatalog catalog;
    Engine::ObjectFactory::createObjectCatalog( configPath(), catalog );

    ASSERT_NE( catalog.object( 1 ), nullptr );
    EXPECT_TRUE( catalog.object( 1 )->isWalkable() );
    EXPECT_EQ( catalog.maxBlockingFootprintWidth(), 0 );
    EXPECT_EQ( catalog.maxBlockingFootprintHeight(), 0 );
}

TEST_F( ObjectFactoryTest, SaveObjectCatalog_NotWalkable_ThenCreateObjectCatalog_RoundTripsAndTracksMaxFootprint ) {
    Engine::ObjectCatalog original;
    Engine::ObjectModel object;
    object.setType( 5 );
    object.setName( "Tree" );
    object.setTextureFile( "Textures/Objects/tree_01" );
    Engine::ObjectSizeModel size;
    size.setWidth( 2 );
    size.setHeight( 3 );
    object.setSize( size );
    object.setIsWalkable( false );
    original.addObject( object );

    Engine::ObjectFactory::saveObjectCatalog( configPath(), original );

    Engine::ObjectCatalog reloaded;
    Engine::ObjectFactory::createObjectCatalog( configPath(), reloaded );

    ASSERT_NE( reloaded.object( 5 ), nullptr );
    EXPECT_FALSE( reloaded.object( 5 )->isWalkable() );
    EXPECT_EQ( reloaded.maxBlockingFootprintWidth(), 2 );
    EXPECT_EQ( reloaded.maxBlockingFootprintHeight(), 3 );
}

TEST_P( ObjectFactoryInteractionTest, CreateObjectCatalog_Interaction ) {
    Json::Value object;
    object[ "Type" ] = 1;
    object[ "Name" ] = "Object";
    object[ "TextureFile" ] = "Textures/Objects/object_01";
    if ( GetParam().present ) {
        object[ "Interaction" ] = GetParam().interaction;
    }
    if ( GetParam().containerCapacity >= 0 ) {
        object[ "ContainerCapacity" ] = GetParam().containerCapacity;
    }

    Json::Value objects( Json::arrayValue );
    objects.append( object );
    writeObjectsJson( objects );

    Engine::ObjectCatalog catalog;
    Engine::ObjectFactory::createObjectCatalog( configPath(), catalog );

    ASSERT_NE( catalog.object( 1 ), nullptr );

    const std::optional<Engine::ObjectInteractionModel>& interaction = catalog.object( 1 )->interaction();

    ASSERT_EQ( interaction.has_value(), GetParam().expectedPresent );
    if ( GetParam().expectedPresent ) {
        EXPECT_EQ( interaction->type(), GetParam().expectedType );
        EXPECT_EQ( interaction->containerCapacity(), GetParam().expectedCapacity );
    }
}

INSTANTIATE_TEST_SUITE_P(
    Values,
    ObjectFactoryInteractionTest,
    ::testing::Values(
        InteractionCase( "Missing_IsEmpty", false, "", -1, false, Engine::ObjectInteractionEnum::CONTAINER, 0 ),
        InteractionCase( "UnknownName_IsEmpty", true, "LEVER", -1, false, Engine::ObjectInteractionEnum::CONTAINER, 0 ),
        InteractionCase( "LowercaseName_IsEmpty", true, "container", 8, false, Engine::ObjectInteractionEnum::CONTAINER, 0 ),
        InteractionCase( "Container_ParsesCapacity", true, "CONTAINER", 8, true, Engine::ObjectInteractionEnum::CONTAINER, 8 ),
        InteractionCase( "Container_ParsesLargeCapacity", true, "CONTAINER", 40, true, Engine::ObjectInteractionEnum::CONTAINER, 40 ) ),
    []( const ::testing::TestParamInfo<InteractionCase>& info ) { return info.param.name; } );

TEST_P( ObjectFactoryWrongTypeTest, CreateObjectCatalog_WrongFieldType_SkipsEntryAndKeepsOthers ) {
    Json::Value invalid;
    invalid[ "Type" ] = 1;
    invalid[ "Name" ] = "Bad";
    invalid[ "TextureFile" ] = "Textures/Objects/bad";
    if ( GetParam().parent.empty() ) {
        invalid[ GetParam().key ] = GetParam().value;
    } else {
        invalid[ GetParam().parent ][ GetParam().key ] = GetParam().value;
    }

    Json::Value valid;
    valid[ "Type" ] = 2;
    valid[ "Name" ] = "Tree";
    valid[ "TextureFile" ] = "Textures/Objects/tree_01";

    Json::Value objects( Json::arrayValue );
    objects.append( invalid );
    objects.append( valid );
    writeObjectsJson( objects );

    Engine::ObjectCatalog catalog;
    Engine::ObjectFactory::createObjectCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.object( 1 ), nullptr );
    EXPECT_NE( catalog.object( 2 ), nullptr );
    EXPECT_EQ( catalog.objects().size(), 1u );
}

INSTANTIATE_TEST_SUITE_P(
    Fields,
    ObjectFactoryWrongTypeTest,
    ::testing::Values(
        ObjectWrongTypeCase( "TypeIsText", "", "Type", Json::Value( "one" ) ),
        ObjectWrongTypeCase( "NameIsNumber", "", "Name", Json::Value( 5 ) ),
        ObjectWrongTypeCase( "TextureFileIsNumber", "", "TextureFile", Json::Value( 5 ) ),
        ObjectWrongTypeCase( "FootprintIsNumber", "", "Footprint", Json::Value( 2 ) ),
        ObjectWrongTypeCase( "FootprintWidthIsText", "Footprint", "Width", Json::Value( "wide" ) ),
        ObjectWrongTypeCase( "FootprintHeightIsFraction", "Footprint", "Height", Json::Value( 1.5 ) ),
        ObjectWrongTypeCase( "IsWalkableIsText", "", "IsWalkable", Json::Value( "no" ) ),
        ObjectWrongTypeCase( "InteractionIsNumber", "", "Interaction", Json::Value( 3 ) ),
        ObjectWrongTypeCase( "ContainerCapacityIsText", "", "ContainerCapacity", Json::Value( "eight" ) ) ),
    []( const ::testing::TestParamInfo<ObjectWrongTypeCase>& info ) { return info.param.name; } );

TEST_P( ObjectFactoryFootprintTest, CreateObjectCatalog_Footprint ) {
    Json::Value object;
    object[ "Type" ] = 1;
    object[ "Name" ] = "Object";
    object[ "TextureFile" ] = "Textures/Objects/object_01";
    if ( GetParam().present ) {
        object[ "Footprint" ][ "Width" ] = GetParam().width;
        object[ "Footprint" ][ "Height" ] = GetParam().height;
    }

    Json::Value objects( Json::arrayValue );
    objects.append( object );
    writeObjectsJson( objects );

    Engine::ObjectCatalog catalog;
    Engine::ObjectFactory::createObjectCatalog( configPath(), catalog );

    ASSERT_NE( catalog.object( 1 ), nullptr );
    EXPECT_EQ( catalog.object( 1 )->size().width(), GetParam().expectedWidth );
    EXPECT_EQ( catalog.object( 1 )->size().height(), GetParam().expectedHeight );
}

INSTANTIATE_TEST_SUITE_P(
    Values,
    ObjectFactoryFootprintTest,
    ::testing::Values(
        FootprintCase( "Missing_DefaultsToOneByOne", false, 0, 0, 1, 1 ),
        FootprintCase( "TwoByThree", true, 2, 3, 2, 3 ),
        FootprintCase( "WideStrip", true, 4, 1, 4, 1 ),
        FootprintCase( "TallStrip", true, 1, 4, 1, 4 ) ),
    []( const ::testing::TestParamInfo<FootprintCase>& info ) { return info.param.name; } );

TEST_F( ObjectFactoryTest, SaveObjectCatalog_ContainerInteraction_ThenCreateObjectCatalog_RoundTrips ) {
    Engine::ObjectCatalog original;
    Engine::ObjectModel object;
    object.setType( 5 );
    object.setName( "TreasureChest" );
    object.setTextureFile( "Textures/Objects/treasure_chest_01" );

    Engine::ObjectInteractionModel interaction;
    interaction.setType( Engine::ObjectInteractionEnum::CONTAINER );
    interaction.setContainerCapacity( 8 );
    object.setInteraction( interaction );
    original.addObject( object );

    Engine::ObjectFactory::saveObjectCatalog( configPath(), original );

    Engine::ObjectCatalog reloaded;
    Engine::ObjectFactory::createObjectCatalog( configPath(), reloaded );

    ASSERT_NE( reloaded.object( 5 ), nullptr );
    ASSERT_TRUE( reloaded.object( 5 )->interaction().has_value() );
    EXPECT_EQ( reloaded.object( 5 )->interaction()->type(), Engine::ObjectInteractionEnum::CONTAINER );
    EXPECT_EQ( reloaded.object( 5 )->interaction()->containerCapacity(), 8u );
}
