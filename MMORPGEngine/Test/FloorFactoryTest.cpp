#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <memory>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/World/FloorFactory.h>
#include <MMORPGEngine/World/WorldModel.h>

namespace {

class FloorFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_floorfactory";
        std::filesystem::create_directories( _tempDir );
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    std::string floorPath( const std::string& fileName ) const {
        return ( _tempDir / fileName ).string();
    }

    std::filesystem::path _tempDir;
};

} // namespace

TEST_F( FloorFactoryTest, CreateEmptyFloor_ThenDeleteFloor_RoundTrips ) {
    const std::string path = floorPath( "empty.json" );

    ASSERT_TRUE( Engine::FloorFactory::createEmptyFloor( path, 3, 2, 2 ) );

    const Json::Value floorJson = Engine::JsonHelper::loadJsonFile( path );
    EXPECT_EQ( floorJson[ "Z" ].asInt(), 3 );
    ASSERT_EQ( floorJson[ "Tiles" ].size(), 2u );
    EXPECT_EQ( floorJson[ "Tiles" ][ 0 ].size(), 2u );
    EXPECT_EQ( floorJson[ "Tiles" ][ 0 ][ 0 ].asUInt(), 0u );
    ASSERT_EQ( floorJson[ "Objects" ].size(), 2u );

    Engine::FloorFactory::deleteFloor( path );

    EXPECT_FALSE( std::filesystem::exists( path ) );
}

TEST_F( FloorFactoryTest, CreateEmptyFloor_NonexistentDirectory_ReturnsFalse ) {
    const std::string path = ( _tempDir / "missing_subdir" / "floor.json" ).string();

    EXPECT_FALSE( Engine::FloorFactory::createEmptyFloor( path, 0, 2, 2 ) );
}

TEST_F( FloorFactoryTest, SaveFloor_PreservesExistingZ_AndWritesWorldGrid ) {
    const std::string path = floorPath( "save.json" );
    ASSERT_TRUE( Engine::FloorFactory::createEmptyFloor( path, 5, 2, 2 ) );

    Engine::WorldModel world;
    world.setWidth( 2 );
    world.setHeight( 2 );

    Engine::ChunkModel* chunk = world.chunk( 0, 0 );

    auto tile = std::make_unique<Engine::WorldTileModel>();
    tile->setTileType( 7 );
    chunk->setTile( 1, 0, 5, std::move( tile ) );

    auto object = std::make_unique<Engine::WorldObjectModel>();
    object->setObjectType( 9 );
    chunk->setObject( 0, 1, 5, std::move( object ) );

    Engine::FloorFactory::saveFloor( path, world );

    const Json::Value savedJson = Engine::JsonHelper::loadJsonFile( path );
    EXPECT_EQ( savedJson[ "Z" ].asInt(), 5 );
    EXPECT_EQ( savedJson[ "Tiles" ][ 0 ][ 0 ].asUInt(), 0u );
    EXPECT_EQ( savedJson[ "Tiles" ][ 0 ][ 1 ].asUInt(), 7u );
    EXPECT_EQ( savedJson[ "Objects" ][ 1 ][ 0 ].asUInt(), 9u );
    EXPECT_EQ( savedJson[ "Objects" ][ 0 ][ 0 ].asUInt(), 0u );
}

TEST_F( FloorFactoryTest, CreateFloor_NullWorld_DoesNotCrash ) {
    EXPECT_NO_THROW( Engine::FloorFactory::createFloor( floorPath( "unused.json" ), nullptr ) );
}

TEST_F( FloorFactoryTest, CreateFloor_UnknownTypeInExistingChunk_YieldsDefaultTypeZero ) {
    Engine::TileModel tile;
    tile.setType( 9101 );
    tile.setName( "FloorFactoryTestTile" );
    Engine::Singleton<Engine::DataManager>::instance().addTile( tile );

    Engine::ObjectModel object;
    object.setType( 9102 );
    object.setName( "FloorFactoryTestObject" );
    Engine::Singleton<Engine::DataManager>::instance().addObject( object );

    Json::Value floorJson;
    floorJson[ "Z" ] = 2;
    Json::Value tilesRow( Json::arrayValue );
    tilesRow.append( 9101 );
    tilesRow.append( 424242 );
    floorJson[ "Tiles" ].append( tilesRow );
    Json::Value objectsRow( Json::arrayValue );
    objectsRow.append( 9102 );
    objectsRow.append( 0 );
    floorJson[ "Objects" ].append( objectsRow );

    const std::string path = floorPath( "populated.json" );
    ASSERT_TRUE( Engine::JsonHelper::saveJsonFile( path, floorJson ) );

    Engine::WorldModel world;
    Engine::FloorFactory::createFloor( path, &world );

    const std::vector<int> floors = world.floors();
    EXPECT_NE( std::find( floors.begin(), floors.end(), 2 ), floors.end() );

    ASSERT_NE( world.tile( 0, 0, 2 ), nullptr );
    EXPECT_EQ( world.tile( 0, 0, 2 )->tileType(), 9101u );

    ASSERT_NE( world.tile( 1, 0, 2 ), nullptr );
    EXPECT_EQ( world.tile( 1, 0, 2 )->tileType(), 0u );

    ASSERT_NE( world.object( 0, 0, 2 ), nullptr );
    EXPECT_EQ( world.object( 0, 0, 2 )->objectType(), 9102u );

    ASSERT_NE( world.object( 1, 0, 2 ), nullptr );
    EXPECT_EQ( world.object( 1, 0, 2 )->objectType(), 0u );
}

TEST_F( FloorFactoryTest, CreateFloor_TileTypeZero_YieldsEmptyTileWithoutCatalogLookup ) {
    Json::Value floorJson;
    floorJson[ "Z" ] = 4;
    Json::Value tilesRow( Json::arrayValue );
    tilesRow.append( 0 );
    floorJson[ "Tiles" ].append( tilesRow );
    floorJson[ "Objects" ].append( Json::Value( Json::arrayValue ) );

    const std::string path = floorPath( "empty_tile.json" );
    ASSERT_TRUE( Engine::JsonHelper::saveJsonFile( path, floorJson ) );

    Engine::WorldModel world;
    Engine::FloorFactory::createFloor( path, &world );

    const Engine::WorldModel& constWorld = world;
    EXPECT_EQ( constWorld.chunk( 0, 0 ), nullptr );
}
