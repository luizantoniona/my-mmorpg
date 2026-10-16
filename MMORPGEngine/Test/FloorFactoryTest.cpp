#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <memory>
#include <string>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/Creature/CreatureSpawnAreaModel.h>
#include <MMORPGEngine/Data/Creature/CreatureSpawnEntryModel.h>
#include <MMORPGEngine/Data/Creature/CreatureTypeModel.h>
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

class InvalidSpawnAreaCase {
public:
    InvalidSpawnAreaCase( std::string name, bool includeX, bool includeY, bool includeWidth, bool includeHeight, Json::Value respawnSeconds ) :
        name( std::move( name ) ),
        includeX( includeX ),
        includeY( includeY ),
        includeWidth( includeWidth ),
        includeHeight( includeHeight ),
        respawnSeconds( std::move( respawnSeconds ) ) {
    }

    std::string name;
    bool includeX;
    bool includeY;
    bool includeWidth;
    bool includeHeight;
    Json::Value respawnSeconds;
};

class SpawnAreaWrongTypeCase {
public:
    SpawnAreaWrongTypeCase( std::string name, std::string key, Json::Value value ) :
        name( std::move( name ) ),
        key( std::move( key ) ),
        value( std::move( value ) ) {
    }

    std::string name;
    std::string key;
    Json::Value value;
};

class FloorFactorySpawnAreaWrongTypeTest : public FloorFactoryTest, public ::testing::WithParamInterface<SpawnAreaWrongTypeCase> {};
class FloorFactoryInvalidSpawnAreaTest : public FloorFactoryTest, public ::testing::WithParamInterface<InvalidSpawnAreaCase> {};

} // namespace

TEST_F( FloorFactoryTest, CreateEmptyFloor_ThenDeleteFloor_RoundTrips ) {
    const std::string path = floorPath( "empty.json" );

    ASSERT_TRUE( Engine::FloorFactory::createEmptyFloor( path, 3, 2, 2 ) );

    const Json::Value floorJson = Engine::JsonHelper::loadJsonFile( path );
    EXPECT_EQ( floorJson[ "Z" ].asInt(), 3 );
    ASSERT_EQ( floorJson[ "Tiles" ].size(), 2u );
    EXPECT_EQ( floorJson[ "Tiles" ][ 0 ].size(), 2u );
    EXPECT_EQ( floorJson[ "Tiles" ][ 0 ][ 0 ].asUInt(), 0u );
    EXPECT_EQ( floorJson[ "Objects" ].size(), 0u );
    EXPECT_EQ( floorJson[ "SpawnAreas" ].size(), 0u );

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

    Engine::CreatureSpawnEntryModel entry;
    entry.setType( "GOBLIN" );
    entry.setQuantity( 3 );
    Engine::CreatureSpawnAreaModel area;
    area.setX( 0 );
    area.setY( 0 );
    area.setWidth( 2 );
    area.setHeight( 2 );
    area.setRespawnSeconds( 30.0 );
    area.setCreatures( { entry } );
    world.addSpawnArea( 5, area );

    Engine::FloorFactory::saveFloor( path, world );

    const Json::Value savedJson = Engine::JsonHelper::loadJsonFile( path );
    EXPECT_EQ( savedJson[ "Z" ].asInt(), 5 );
    EXPECT_EQ( savedJson[ "Tiles" ][ 0 ][ 0 ].asUInt(), 0u );
    EXPECT_EQ( savedJson[ "Tiles" ][ 0 ][ 1 ].asUInt(), 7u );
    ASSERT_EQ( savedJson[ "Objects" ].size(), 1u );
    EXPECT_EQ( savedJson[ "Objects" ][ 0 ][ "X" ].asUInt(), 0u );
    EXPECT_EQ( savedJson[ "Objects" ][ 0 ][ "Y" ].asUInt(), 1u );
    EXPECT_EQ( savedJson[ "Objects" ][ 0 ][ "Type" ].asUInt(), 9u );
    ASSERT_EQ( savedJson[ "SpawnAreas" ].size(), 1u );
    EXPECT_EQ( savedJson[ "SpawnAreas" ][ 0 ][ "X" ].asInt(), 0 );
    EXPECT_EQ( savedJson[ "SpawnAreas" ][ 0 ][ "Width" ].asUInt(), 2u );
    EXPECT_DOUBLE_EQ( savedJson[ "SpawnAreas" ][ 0 ][ "RespawnSeconds" ].asDouble(), 30.0 );
    ASSERT_EQ( savedJson[ "SpawnAreas" ][ 0 ][ "Creatures" ].size(), 1u );
    EXPECT_EQ( savedJson[ "SpawnAreas" ][ 0 ][ "Creatures" ][ 0 ][ "Type" ].asString(), "GOBLIN" );
    EXPECT_EQ( savedJson[ "SpawnAreas" ][ 0 ][ "Creatures" ][ 0 ][ "Quantity" ].asUInt(), 3u );
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
    Json::Value objectEntry;
    objectEntry[ "X" ] = 0;
    objectEntry[ "Y" ] = 0;
    objectEntry[ "Type" ] = 9102;
    floorJson[ "Objects" ].append( objectEntry );

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

TEST_F( FloorFactoryTest, SaveFloor_ThenCreateFloor_RoundTripsOverlays ) {
    Engine::TileModel base;
    base.setType( 9111 );
    base.setName( "FloorFactoryTestBase" );
    Engine::Singleton<Engine::DataManager>::instance().addTile( base );

    Engine::TileModel overlay;
    overlay.setType( 9112 );
    overlay.setName( "FloorFactoryTestOverlay" );
    Engine::Singleton<Engine::DataManager>::instance().addTile( overlay );

    const std::string path = floorPath( "overlay.json" );
    ASSERT_TRUE( Engine::FloorFactory::createEmptyFloor( path, 1, 2, 2 ) );

    Engine::WorldModel world;
    world.setWidth( 2 );
    world.setHeight( 2 );
    world.setTile( 1, 1, 1, 9111 );
    world.setOverlay( 1, 1, 1, 9112 );

    Engine::FloorFactory::saveFloor( path, world );

    const Json::Value savedJson = Engine::JsonHelper::loadJsonFile( path );
    ASSERT_EQ( savedJson[ "Overlays" ].size(), 1u );
    EXPECT_EQ( savedJson[ "Overlays" ][ 0 ][ "X" ].asInt(), 1 );
    EXPECT_EQ( savedJson[ "Overlays" ][ 0 ][ "Y" ].asInt(), 1 );
    EXPECT_EQ( savedJson[ "Overlays" ][ 0 ][ "Type" ].asUInt(), 9112u );

    Engine::WorldModel loaded;
    Engine::FloorFactory::createFloor( path, &loaded );

    ASSERT_NE( loaded.tile( 1, 1, 1 ), nullptr );
    EXPECT_EQ( loaded.tile( 1, 1, 1 )->tileType(), 9111u );
    EXPECT_EQ( loaded.tile( 1, 1, 1 )->overlayType(), 9112u );
}

TEST_F( FloorFactoryTest, CreateFloor_FloorWithoutOverlays_LoadsWithoutOverlay ) {
    Json::Value floorJson;
    floorJson[ "Z" ] = 6;
    Json::Value tilesRow( Json::arrayValue );
    tilesRow.append( 0 );
    floorJson[ "Tiles" ].append( tilesRow );

    const std::string path = floorPath( "no_overlay.json" );
    ASSERT_TRUE( Engine::JsonHelper::saveJsonFile( path, floorJson ) );

    Engine::WorldModel world;
    EXPECT_NO_THROW( Engine::FloorFactory::createFloor( path, &world ) );

    const Engine::WorldModel& constWorld = world;
    EXPECT_EQ( constWorld.chunk( 0, 0 ), nullptr );
}

TEST_F( FloorFactoryTest, CreateFloor_TileTypeZero_YieldsEmptyTileWithoutCatalogLookup ) {
    Json::Value floorJson;
    floorJson[ "Z" ] = 4;
    Json::Value tilesRow( Json::arrayValue );
    tilesRow.append( 0 );
    floorJson[ "Tiles" ].append( tilesRow );
    floorJson[ "Objects" ] = Json::Value( Json::arrayValue );

    const std::string path = floorPath( "empty_tile.json" );
    ASSERT_TRUE( Engine::JsonHelper::saveJsonFile( path, floorJson ) );

    Engine::WorldModel world;
    Engine::FloorFactory::createFloor( path, &world );

    const Engine::WorldModel& constWorld = world;
    EXPECT_EQ( constWorld.chunk( 0, 0 ), nullptr );
}

TEST_F( FloorFactoryTest, CreateFloor_SpawnArea_ParsesFieldsAndSkipsUnknownCreature ) {
    Engine::CreatureTypeModel creatureType;
    creatureType.setType( "FLOOR_FACTORY_TEST_CREATURE" );
    creatureType.setName( "FloorFactoryTestCreature" );
    Engine::Singleton<Engine::DataManager>::instance().addCreatureType( creatureType );

    Json::Value floorJson;
    floorJson[ "Z" ] = 6;
    floorJson[ "Tiles" ] = Json::Value( Json::arrayValue );
    floorJson[ "Objects" ] = Json::Value( Json::arrayValue );

    Json::Value area;
    area[ "X" ] = 10;
    area[ "Y" ] = 20;
    area[ "Width" ] = 5;
    area[ "Height" ] = 5;
    area[ "RespawnSeconds" ] = 45.0;
    Json::Value knownEntry;
    knownEntry[ "Type" ] = "FLOOR_FACTORY_TEST_CREATURE";
    knownEntry[ "Quantity" ] = 2;
    Json::Value unknownEntry;
    unknownEntry[ "Type" ] = "UNKNOWN_CREATURE";
    unknownEntry[ "Quantity" ] = 1;
    area[ "Creatures" ].append( knownEntry );
    area[ "Creatures" ].append( unknownEntry );
    floorJson[ "SpawnAreas" ].append( area );

    const std::string path = floorPath( "spawn_area.json" );
    ASSERT_TRUE( Engine::JsonHelper::saveJsonFile( path, floorJson ) );

    Engine::WorldModel world;
    Engine::FloorFactory::createFloor( path, &world );

    const std::vector<Engine::CreatureSpawnAreaModel> areas = world.spawnAreas( 6 );
    ASSERT_EQ( areas.size(), 1u );
    EXPECT_EQ( areas[ 0 ].x(), 10 );
    EXPECT_EQ( areas[ 0 ].y(), 20 );
    EXPECT_EQ( areas[ 0 ].width(), 5u );
    EXPECT_EQ( areas[ 0 ].height(), 5u );
    EXPECT_DOUBLE_EQ( areas[ 0 ].respawnSeconds(), 45.0 );
    ASSERT_EQ( areas[ 0 ].creatures().size(), 1u );
    EXPECT_EQ( areas[ 0 ].creatures()[ 0 ].type(), "FLOOR_FACTORY_TEST_CREATURE" );
    EXPECT_EQ( areas[ 0 ].creatures()[ 0 ].quantity(), 2u );
}

TEST_P( FloorFactorySpawnAreaWrongTypeTest, CreateFloor_SpawnAreaWithWrongFieldType_IsSkipped ) {
    Json::Value area;
    area[ "X" ] = 0;
    area[ "Y" ] = 0;
    area[ "Width" ] = 2;
    area[ "Height" ] = 2;
    area[ "RespawnSeconds" ] = 30;
    area[ GetParam().key ] = GetParam().value;

    Json::Value floorJson;
    floorJson[ "Z" ] = 8;
    floorJson[ "Tiles" ] = Json::Value( Json::arrayValue );
    floorJson[ "Objects" ] = Json::Value( Json::arrayValue );
    floorJson[ "SpawnAreas" ].append( area );

    const std::string path = floorPath( "wrong_type_spawn_area.json" );
    ASSERT_TRUE( Engine::JsonHelper::saveJsonFile( path, floorJson ) );

    Engine::WorldModel world;
    Engine::FloorFactory::createFloor( path, &world );

    EXPECT_TRUE( world.spawnAreas( 8 ).empty() );
}

INSTANTIATE_TEST_SUITE_P(
    Fields,
    FloorFactorySpawnAreaWrongTypeTest,
    ::testing::Values(
        SpawnAreaWrongTypeCase( "XIsText", "X", Json::Value( "left" ) ),
        SpawnAreaWrongTypeCase( "YIsFraction", "Y", Json::Value( 1.5 ) ),
        SpawnAreaWrongTypeCase( "WidthIsNegative", "Width", Json::Value( -2 ) ),
        SpawnAreaWrongTypeCase( "HeightIsText", "Height", Json::Value( "tall" ) ),
        SpawnAreaWrongTypeCase( "CreaturesIsText", "Creatures", Json::Value( "rats" ) ) ),
    []( const ::testing::TestParamInfo<SpawnAreaWrongTypeCase>& info ) { return info.param.name; } );

TEST_F( FloorFactoryTest, CreateFloor_MalformedGridAndEntries_DoNotThrow ) {
    Json::Value badOverlay;
    badOverlay[ "X" ] = "left";
    badOverlay[ "Y" ] = 0;
    badOverlay[ "Type" ] = 1;

    Json::Value badObject;
    badObject[ "X" ] = 0;
    badObject[ "Y" ] = 0;
    badObject[ "Type" ] = "tree";

    Json::Value floorJson;
    floorJson[ "Z" ] = 9;
    floorJson[ "Tiles" ] = Json::Value( Json::arrayValue );
    floorJson[ "Tiles" ].append( 5 );
    Json::Value row( Json::arrayValue );
    row.append( "grass" );
    row.append( -1 );
    floorJson[ "Tiles" ].append( row );
    floorJson[ "Overlays" ] = Json::Value( Json::arrayValue );
    floorJson[ "Overlays" ].append( badOverlay );
    floorJson[ "Overlays" ].append( 7 );
    floorJson[ "Objects" ] = Json::Value( Json::arrayValue );
    floorJson[ "Objects" ].append( badObject );
    floorJson[ "Objects" ].append( "tree" );
    floorJson[ "SpawnAreas" ] = Json::Value( Json::arrayValue );
    floorJson[ "SpawnAreas" ].append( 7 );

    const std::string path = floorPath( "malformed_floor.json" );
    ASSERT_TRUE( Engine::JsonHelper::saveJsonFile( path, floorJson ) );

    Engine::WorldModel world;

    EXPECT_NO_THROW( Engine::FloorFactory::createFloor( path, &world ) );
    EXPECT_TRUE( world.spawnAreas( 9 ).empty() );
}

TEST_P( FloorFactoryInvalidSpawnAreaTest, CreateFloor_InvalidSpawnArea_IsSkipped ) {
    Json::Value area;
    if ( GetParam().includeX ) {
        area[ "X" ] = 0;
    }
    if ( GetParam().includeY ) {
        area[ "Y" ] = 0;
    }
    if ( GetParam().includeWidth ) {
        area[ "Width" ] = 2;
    }
    if ( GetParam().includeHeight ) {
        area[ "Height" ] = 2;
    }
    if ( GetParam().respawnSeconds.isNumeric() || GetParam().respawnSeconds.isString() ) {
        area[ "RespawnSeconds" ] = GetParam().respawnSeconds;
    }

    Json::Value floorJson;
    floorJson[ "Z" ] = 8;
    floorJson[ "Tiles" ] = Json::Value( Json::arrayValue );
    floorJson[ "Objects" ] = Json::Value( Json::arrayValue );
    floorJson[ "SpawnAreas" ].append( area );

    const std::string path = floorPath( "invalid_spawn_area.json" );
    ASSERT_TRUE( Engine::JsonHelper::saveJsonFile( path, floorJson ) );

    Engine::WorldModel world;
    Engine::FloorFactory::createFloor( path, &world );

    EXPECT_TRUE( world.spawnAreas( 8 ).empty() );
}

INSTANTIATE_TEST_SUITE_P(
    Areas,
    FloorFactoryInvalidSpawnAreaTest,
    ::testing::Values(
        InvalidSpawnAreaCase( "MissingRespawnSeconds", true, true, true, true, Json::Value( Json::nullValue ) ),
        InvalidSpawnAreaCase( "ZeroRespawnSeconds", true, true, true, true, Json::Value( 0 ) ),
        InvalidSpawnAreaCase( "NegativeRespawnSeconds", true, true, true, true, Json::Value( -5 ) ),
        InvalidSpawnAreaCase( "TextRespawnSeconds", true, true, true, true, Json::Value( "soon" ) ),
        InvalidSpawnAreaCase( "MissingX", false, true, true, true, Json::Value( 30 ) ),
        InvalidSpawnAreaCase( "MissingY", true, false, true, true, Json::Value( 30 ) ),
        InvalidSpawnAreaCase( "MissingWidth", true, true, false, true, Json::Value( 30 ) ),
        InvalidSpawnAreaCase( "MissingHeight", true, true, true, false, Json::Value( 30 ) ) ),
    []( const ::testing::TestParamInfo<InvalidSpawnAreaCase>& info ) { return info.param.name; } );
