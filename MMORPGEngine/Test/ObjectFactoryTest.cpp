#include <gtest/gtest.h>

#include <filesystem>

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

} // namespace

TEST_F( ObjectFactoryTest, CreateObjectCatalog_ParsesFields ) {
    Json::Value object;
    object[ "Type" ] = 1;
    object[ "Name" ] = "StoneWall";
    object[ "TextureFolder" ] = "Textures/Objects/StoneWall";
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
    EXPECT_EQ( catalog.object( 1 )->folder(), "Textures/Objects/StoneWall" );
    EXPECT_EQ( catalog.object( 1 )->size().width(), 2 );
    EXPECT_EQ( catalog.object( 1 )->size().height(), 3 );
}

TEST_F( ObjectFactoryTest, CreateObjectCatalog_MissingFootprint_DefaultsToOneByOne ) {
    Json::Value object;
    object[ "Type" ] = 1;
    object[ "Name" ] = "StoneWall";
    object[ "TextureFolder" ] = "Textures/Objects/StoneWall";

    Json::Value objects( Json::arrayValue );
    objects.append( object );
    writeObjectsJson( objects );

    Engine::ObjectCatalog catalog;
    Engine::ObjectFactory::createObjectCatalog( configPath(), catalog );

    ASSERT_NE( catalog.object( 1 ), nullptr );
    EXPECT_EQ( catalog.object( 1 )->size().width(), 1 );
    EXPECT_EQ( catalog.object( 1 )->size().height(), 1 );
}

TEST_F( ObjectFactoryTest, SaveObjectCatalog_ThenCreateObjectCatalog_RoundTripsFields ) {
    Engine::ObjectCatalog original;
    Engine::ObjectModel object;
    object.setType( 5 );
    object.setName( "WoodenCrate" );
    object.setFolder( "Textures/Objects/WoodenCrate" );
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
    EXPECT_EQ( reloaded.object( 5 )->folder(), "Textures/Objects/WoodenCrate" );
    EXPECT_EQ( reloaded.object( 5 )->size().width(), 2 );
    EXPECT_EQ( reloaded.object( 5 )->size().height(), 1 );
}

TEST_F( ObjectFactoryTest, CreateObjectCatalog_MissingInteraction_IsEmpty ) {
    Json::Value object;
    object[ "Type" ] = 1;
    object[ "Name" ] = "StoneWall";
    object[ "TextureFolder" ] = "Textures/Objects/StoneWall";

    Json::Value objects( Json::arrayValue );
    objects.append( object );
    writeObjectsJson( objects );

    Engine::ObjectCatalog catalog;
    Engine::ObjectFactory::createObjectCatalog( configPath(), catalog );

    ASSERT_NE( catalog.object( 1 ), nullptr );
    EXPECT_FALSE( catalog.object( 1 )->interaction().has_value() );
}

TEST_F( ObjectFactoryTest, CreateObjectCatalog_UnknownInteraction_IsEmpty ) {
    Json::Value object;
    object[ "Type" ] = 1;
    object[ "Name" ] = "StoneWall";
    object[ "TextureFolder" ] = "Textures/Objects/StoneWall";
    object[ "Interaction" ] = "LEVER";

    Json::Value objects( Json::arrayValue );
    objects.append( object );
    writeObjectsJson( objects );

    Engine::ObjectCatalog catalog;
    Engine::ObjectFactory::createObjectCatalog( configPath(), catalog );

    ASSERT_NE( catalog.object( 1 ), nullptr );
    EXPECT_FALSE( catalog.object( 1 )->interaction().has_value() );
}

TEST_F( ObjectFactoryTest, CreateObjectCatalog_ContainerInteraction_ParsesCapacity ) {
    Json::Value object;
    object[ "Type" ] = 1;
    object[ "Name" ] = "TreasureChest";
    object[ "TextureFolder" ] = "Textures/Objects/TreasureChest";
    object[ "Interaction" ] = "CONTAINER";
    object[ "ContainerCapacity" ] = 8;

    Json::Value objects( Json::arrayValue );
    objects.append( object );
    writeObjectsJson( objects );

    Engine::ObjectCatalog catalog;
    Engine::ObjectFactory::createObjectCatalog( configPath(), catalog );

    ASSERT_NE( catalog.object( 1 ), nullptr );
    ASSERT_TRUE( catalog.object( 1 )->interaction().has_value() );
    EXPECT_EQ( catalog.object( 1 )->interaction()->type(), Engine::ObjectInteractionEnum::CONTAINER );
    EXPECT_EQ( catalog.object( 1 )->interaction()->containerCapacity(), 8u );
}

TEST_F( ObjectFactoryTest, SaveObjectCatalog_ContainerInteraction_ThenCreateObjectCatalog_RoundTrips ) {
    Engine::ObjectCatalog original;
    Engine::ObjectModel object;
    object.setType( 5 );
    object.setName( "TreasureChest" );
    object.setFolder( "Textures/Objects/TreasureChest" );

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
