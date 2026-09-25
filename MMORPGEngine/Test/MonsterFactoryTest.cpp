#include <gtest/gtest.h>

#include <filesystem>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Monster/MonsterFactory.h>

namespace {

class MonsterFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_monsterfactory";
        std::filesystem::create_directories( _tempDir / "TestMap" );

        Json::Value configJson;
        configJson[ "ActiveFolder" ] = "TestMap";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "Config.json" ).string(), configJson );

        Json::Value mapJson;
        mapJson[ "Catalogs" ][ "Monsters" ] = "Monster.json";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Map.json" ).string(), mapJson );
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    void writeMonstersJson( const Json::Value& monstersArray ) const {
        Json::Value monsterJson;
        monsterJson[ "Monsters" ] = monstersArray;
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Monster.json" ).string(), monsterJson );
    }

    QString configPath() const {
        return QString::fromStdString( _tempDir.string() ) + "/";
    }

    std::filesystem::path _tempDir;
};

} // namespace

TEST_F( MonsterFactoryTest, CreateMonsterCatalog_ParsesFields ) {
    Json::Value monster;
    monster[ "Type" ] = 1;
    monster[ "Name" ] = "Goblin";

    Json::Value monsters( Json::arrayValue );
    monsters.append( monster );
    writeMonstersJson( monsters );

    Engine::MonsterCatalog catalog;
    Engine::MonsterFactory::createMonsterCatalog( configPath(), catalog );

    ASSERT_NE( catalog.monster( 1 ), nullptr );
    EXPECT_EQ( catalog.monster( 1 )->name(), "Goblin" );
}

TEST_F( MonsterFactoryTest, CreateMonsterCatalog_MultipleMonsters_AllAdded ) {
    Json::Value first;
    first[ "Type" ] = 1;
    first[ "Name" ] = "Goblin";

    Json::Value second;
    second[ "Type" ] = 2;
    second[ "Name" ] = "Rat";

    Json::Value monsters( Json::arrayValue );
    monsters.append( first );
    monsters.append( second );
    writeMonstersJson( monsters );

    Engine::MonsterCatalog catalog;
    Engine::MonsterFactory::createMonsterCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.monsters().size(), 2u );
}

TEST_F( MonsterFactoryTest, CreateMonsterCatalog_MissingName_SkipsEntry ) {
    Json::Value invalid;
    invalid[ "Type" ] = 1;

    Json::Value valid;
    valid[ "Type" ] = 2;
    valid[ "Name" ] = "Rat";

    Json::Value monsters( Json::arrayValue );
    monsters.append( invalid );
    monsters.append( valid );
    writeMonstersJson( monsters );

    Engine::MonsterCatalog catalog;
    Engine::MonsterFactory::createMonsterCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.monster( 1 ), nullptr );
    ASSERT_NE( catalog.monster( 2 ), nullptr );
    EXPECT_EQ( catalog.monsters().size(), 1u );
}

TEST_F( MonsterFactoryTest, SaveMonsterCatalog_ThenCreateMonsterCatalog_RoundTripsFields ) {
    Engine::MonsterCatalog original;
    Engine::MonsterModel monster;
    monster.setType( 1 );
    monster.setName( "Goblin" );
    original.addMonster( monster );

    Engine::MonsterFactory::saveMonsterCatalog( configPath(), original );

    Engine::MonsterCatalog reloaded;
    Engine::MonsterFactory::createMonsterCatalog( configPath(), reloaded );

    ASSERT_NE( reloaded.monster( 1 ), nullptr );
    EXPECT_EQ( reloaded.monster( 1 )->name(), "Goblin" );
}
