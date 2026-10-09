#include <gtest/gtest.h>

#include <filesystem>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Creature/CreatureTypeFactory.h>

namespace {

class CreatureTypeFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_creaturetypefactory";
        std::filesystem::create_directories( _tempDir / "TestMap" );

        Json::Value configJson;
        configJson[ "ActiveFolder" ] = "TestMap";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "Config.json" ).string(), configJson );

        Json::Value mapJson;
        mapJson[ "Catalogs" ][ "Creatures" ] = "Creatures.json";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Map.json" ).string(), mapJson );
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    void writeIndex( const std::vector<std::string>& keys ) const {
        Json::Value indexJson;
        indexJson[ "Creatures" ] = Json::Value( Json::arrayValue );
        for ( const std::string& key : keys ) {
            indexJson[ "Creatures" ].append( key );
        }
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Creatures.json" ).string(), indexJson );
    }

    void writeCreature( const std::string& key, const Json::Value& creatureJson ) const {
        std::filesystem::create_directories( _tempDir / "TestMap" / "Creatures" / key );
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Creatures" / key / ( key + ".json" ) ).string(), creatureJson );
    }

    QString configPath() const {
        return QString::fromStdString( _tempDir.string() ) + "/";
    }

    std::filesystem::path _tempDir;
};

} // namespace

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_ParsesFields ) {
    Json::Value creatureType;
    creatureType[ "Name" ] = "Goblin";
    creatureType[ "Vitals" ][ "MaxHealth" ] = 10;
    creatureType[ "AggroRadius" ] = 5;

    writeIndex( { "GOBLIN" } );
    writeCreature( "GOBLIN", creatureType );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    ASSERT_NE( catalog.creatureType( "GOBLIN" ), nullptr );
    EXPECT_EQ( catalog.creatureType( "GOBLIN" )->type(), "GOBLIN" );
    EXPECT_EQ( catalog.creatureType( "GOBLIN" )->name(), "Goblin" );
    EXPECT_DOUBLE_EQ( catalog.creatureType( "GOBLIN" )->vitals().maxHealth(), 10.0 );
    EXPECT_EQ( catalog.creatureType( "GOBLIN" )->aggroRadius(), 5u );
}

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_MissingVitals_SkipsEntry ) {
    Json::Value invalid;
    invalid[ "Name" ] = "Goblin";

    Json::Value valid;
    valid[ "Name" ] = "Rat";
    valid[ "Vitals" ][ "MaxHealth" ] = 5;

    writeIndex( { "GOBLIN", "RAT" } );
    writeCreature( "GOBLIN", invalid );
    writeCreature( "RAT", valid );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.creatureType( "GOBLIN" ), nullptr );
    ASSERT_NE( catalog.creatureType( "RAT" ), nullptr );
    EXPECT_EQ( catalog.creatureTypes().size(), 1u );
}

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_MultipleCreatureTypes_AllAdded ) {
    Json::Value first;
    first[ "Name" ] = "Goblin";
    first[ "Vitals" ][ "MaxHealth" ] = 10;

    Json::Value second;
    second[ "Name" ] = "Rat";
    second[ "Vitals" ][ "MaxHealth" ] = 5;

    writeIndex( { "GOBLIN", "RAT" } );
    writeCreature( "GOBLIN", first );
    writeCreature( "RAT", second );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.creatureTypes().size(), 2u );
}

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_MissingName_SkipsEntry ) {
    Json::Value invalid;
    invalid[ "Vitals" ][ "MaxHealth" ] = 10;

    Json::Value valid;
    valid[ "Name" ] = "Rat";
    valid[ "Vitals" ][ "MaxHealth" ] = 5;

    writeIndex( { "GOBLIN", "RAT" } );
    writeCreature( "GOBLIN", invalid );
    writeCreature( "RAT", valid );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.creatureType( "GOBLIN" ), nullptr );
    ASSERT_NE( catalog.creatureType( "RAT" ), nullptr );
    EXPECT_EQ( catalog.creatureTypes().size(), 1u );
}

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_IndexedKeyWithoutFile_IsSkipped ) {
    Json::Value valid;
    valid[ "Name" ] = "Rat";
    valid[ "Vitals" ][ "MaxHealth" ] = 5;

    writeIndex( { "GOBLIN", "RAT" } );
    writeCreature( "RAT", valid );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.creatureType( "GOBLIN" ), nullptr );
    EXPECT_NE( catalog.creatureType( "RAT" ), nullptr );
}

TEST_F( CreatureTypeFactoryTest, ReadTypeKeys_InvalidAndDuplicatedKeys_AreSkipped ) {
    writeIndex( { "GOBLIN", "goblin", "GOBLIN", "BAD KEY", "RAT" } );

    const QStringList keys = Engine::CreatureTypeFactory::readTypeKeys( configPath() );

    EXPECT_EQ( keys, QStringList( { "GOBLIN", "RAT" } ) );
}

TEST_F( CreatureTypeFactoryTest, SaveCreatureTypeCatalog_ThenCreateCreatureTypeCatalog_RoundTripsFields ) {
    Engine::CreatureTypeCatalog original;
    Engine::CreatureTypeModel creatureType;
    creatureType.setType( "GOBLIN" );
    creatureType.setName( "Goblin" );
    creatureType.vitals().setMaxHealth( 10 );
    creatureType.setAggroRadius( 5 );
    creatureType.setAttackRange( 4 );
    original.addCreatureType( creatureType );

    Engine::CreatureTypeFactory::saveCreatureTypeCatalog( configPath(), original );

    EXPECT_TRUE( std::filesystem::exists( _tempDir / "TestMap" / "Creatures" / "GOBLIN" / "GOBLIN.json" ) );

    Engine::CreatureTypeCatalog reloaded;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), reloaded );

    ASSERT_NE( reloaded.creatureType( "GOBLIN" ), nullptr );
    EXPECT_EQ( reloaded.creatureType( "GOBLIN" )->name(), "Goblin" );
    EXPECT_DOUBLE_EQ( reloaded.creatureType( "GOBLIN" )->vitals().maxHealth(), 10.0 );
    EXPECT_EQ( reloaded.creatureType( "GOBLIN" )->aggroRadius(), 5u );
    EXPECT_EQ( reloaded.creatureType( "GOBLIN" )->attackRange(), 4u );
}

TEST_F( CreatureTypeFactoryTest, SaveCreatureTypeCatalog_PreservesUnknownFieldsInCreatureFile ) {
    Json::Value existing;
    existing[ "Name" ] = "Goblin";
    existing[ "Vitals" ][ "MaxHealth" ] = 10;
    existing[ "Appearance" ][ "FrameSize" ][ 0 ] = 64;

    writeIndex( { "GOBLIN" } );
    writeCreature( "GOBLIN", existing );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );
    Engine::CreatureTypeFactory::saveCreatureTypeCatalog( configPath(), catalog );

    const Json::Value saved = Engine::JsonHelper::loadJsonFile( ( _tempDir / "TestMap" / "Creatures" / "GOBLIN" / "GOBLIN.json" ).string() );

    EXPECT_EQ( saved[ "Appearance" ][ "FrameSize" ][ 0 ].asInt(), 64 );
}

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_MissingAttackRange_DefaultsToMelee ) {
    Json::Value creatureType;
    creatureType[ "Name" ] = "Rat";
    creatureType[ "Vitals" ][ "MaxHealth" ] = 3;

    writeIndex( { "RAT" } );
    writeCreature( "RAT", creatureType );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    ASSERT_NE( catalog.creatureType( "RAT" ), nullptr );
    EXPECT_EQ( catalog.creatureType( "RAT" )->attackRange(), 1u );
}

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_ParsesFootprint_AndDefaultsToOne ) {
    Json::Value cow;
    cow[ "Name" ] = "Cow";
    cow[ "Vitals" ][ "MaxHealth" ] = 30;
    cow[ "Footprint" ] = 3;

    Json::Value rat;
    rat[ "Name" ] = "Rat";
    rat[ "Vitals" ][ "MaxHealth" ] = 3;

    writeIndex( { "COW", "RAT" } );
    writeCreature( "COW", cow );
    writeCreature( "RAT", rat );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    ASSERT_NE( catalog.creatureType( "COW" ), nullptr );
    ASSERT_NE( catalog.creatureType( "RAT" ), nullptr );
    EXPECT_EQ( catalog.creatureType( "COW" )->footprint(), 3u );
    EXPECT_EQ( catalog.creatureType( "RAT" )->footprint(), 1u );
}

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_ZeroFootprint_SkipsEntry ) {
    Json::Value invalid;
    invalid[ "Name" ] = "Ghost";
    invalid[ "Vitals" ][ "MaxHealth" ] = 3;
    invalid[ "Footprint" ] = 0;

    writeIndex( { "GHOST" } );
    writeCreature( "GHOST", invalid );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.creatureType( "GHOST" ), nullptr );
}

TEST_F( CreatureTypeFactoryTest, SaveCreatureTypeCatalog_RoundTripsFootprint ) {
    Engine::CreatureTypeCatalog original;
    Engine::CreatureTypeModel creatureType;
    creatureType.setType( "COW" );
    creatureType.setName( "Cow" );
    creatureType.vitals().setMaxHealth( 30 );
    creatureType.setFootprint( 3 );
    original.addCreatureType( creatureType );

    Engine::CreatureTypeFactory::saveCreatureTypeCatalog( configPath(), original );

    Engine::CreatureTypeCatalog reloaded;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), reloaded );

    ASSERT_NE( reloaded.creatureType( "COW" ), nullptr );
    EXPECT_EQ( reloaded.creatureType( "COW" )->footprint(), 3u );
}

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_MissingAggroRadius_DefaultsToPassive ) {
    Json::Value creatureType;
    creatureType[ "Name" ] = "Rat";
    creatureType[ "Vitals" ][ "MaxHealth" ] = 3;

    writeIndex( { "RAT" } );
    writeCreature( "RAT", creatureType );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    ASSERT_NE( catalog.creatureType( "RAT" ), nullptr );
    EXPECT_EQ( catalog.creatureType( "RAT" )->aggroRadius(), 0u );
}
