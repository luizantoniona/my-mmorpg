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
        mapJson[ "Catalogs" ][ "Creatures" ] = "Creature.json";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Map.json" ).string(), mapJson );
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    void writeCreaturesJson( const Json::Value& creaturesArray ) const {
        Json::Value creaturesJson;
        creaturesJson[ "Creatures" ] = creaturesArray;
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Creature.json" ).string(), creaturesJson );
    }

    QString configPath() const {
        return QString::fromStdString( _tempDir.string() ) + "/";
    }

    std::filesystem::path _tempDir;
};

} // namespace

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_ParsesFields ) {
    Json::Value creatureType;
    creatureType[ "Type" ] = 1;
    creatureType[ "Name" ] = "Goblin";
    creatureType[ "Vitals" ][ "MaxHealth" ] = 10;

    Json::Value creatureTypes( Json::arrayValue );
    creatureTypes.append( creatureType );
    writeCreaturesJson( creatureTypes );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    ASSERT_NE( catalog.creatureType( 1 ), nullptr );
    EXPECT_EQ( catalog.creatureType( 1 )->name(), "Goblin" );
    EXPECT_DOUBLE_EQ( catalog.creatureType( 1 )->vitals().maxHealth(), 10.0 );
}

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_MissingVitals_SkipsEntry ) {
    Json::Value invalid;
    invalid[ "Type" ] = 1;
    invalid[ "Name" ] = "Goblin";

    Json::Value valid;
    valid[ "Type" ] = 2;
    valid[ "Name" ] = "Rat";
    valid[ "Vitals" ][ "MaxHealth" ] = 5;

    Json::Value creatureTypes( Json::arrayValue );
    creatureTypes.append( invalid );
    creatureTypes.append( valid );
    writeCreaturesJson( creatureTypes );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.creatureType( 1 ), nullptr );
    ASSERT_NE( catalog.creatureType( 2 ), nullptr );
    EXPECT_EQ( catalog.creatureTypes().size(), 1u );
}

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_MultipleCreatureTypes_AllAdded ) {
    Json::Value first;
    first[ "Type" ] = 1;
    first[ "Name" ] = "Goblin";
    first[ "Vitals" ][ "MaxHealth" ] = 10;

    Json::Value second;
    second[ "Type" ] = 2;
    second[ "Name" ] = "Rat";
    second[ "Vitals" ][ "MaxHealth" ] = 5;

    Json::Value creatureTypes( Json::arrayValue );
    creatureTypes.append( first );
    creatureTypes.append( second );
    writeCreaturesJson( creatureTypes );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.creatureTypes().size(), 2u );
}

TEST_F( CreatureTypeFactoryTest, CreateCreatureTypeCatalog_MissingName_SkipsEntry ) {
    Json::Value invalid;
    invalid[ "Type" ] = 1;
    invalid[ "Vitals" ][ "MaxHealth" ] = 10;

    Json::Value valid;
    valid[ "Type" ] = 2;
    valid[ "Name" ] = "Rat";
    valid[ "Vitals" ][ "MaxHealth" ] = 5;

    Json::Value creatureTypes( Json::arrayValue );
    creatureTypes.append( invalid );
    creatureTypes.append( valid );
    writeCreaturesJson( creatureTypes );

    Engine::CreatureTypeCatalog catalog;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.creatureType( 1 ), nullptr );
    ASSERT_NE( catalog.creatureType( 2 ), nullptr );
    EXPECT_EQ( catalog.creatureTypes().size(), 1u );
}

TEST_F( CreatureTypeFactoryTest, SaveCreatureTypeCatalog_ThenCreateCreatureTypeCatalog_RoundTripsFields ) {
    Engine::CreatureTypeCatalog original;
    Engine::CreatureTypeModel creatureType;
    creatureType.setType( 1 );
    creatureType.setName( "Goblin" );
    creatureType.vitals().setMaxHealth( 10 );
    original.addCreatureType( creatureType );

    Engine::CreatureTypeFactory::saveCreatureTypeCatalog( configPath(), original );

    Engine::CreatureTypeCatalog reloaded;
    Engine::CreatureTypeFactory::createCreatureTypeCatalog( configPath(), reloaded );

    ASSERT_NE( reloaded.creatureType( 1 ), nullptr );
    EXPECT_EQ( reloaded.creatureType( 1 )->name(), "Goblin" );
    EXPECT_DOUBLE_EQ( reloaded.creatureType( 1 )->vitals().maxHealth(), 10.0 );
}
