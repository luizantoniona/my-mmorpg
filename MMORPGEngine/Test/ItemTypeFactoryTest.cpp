#include <gtest/gtest.h>

#include <filesystem>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Item/ItemTypeFactory.h>

namespace {

class ItemTypeFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_itemtypefactory";
        std::filesystem::create_directories( _tempDir / "TestMap" );

        Json::Value configJson;
        configJson[ "ActiveFolder" ] = "TestMap";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "Config.json" ).string(), configJson );

        Json::Value mapJson;
        mapJson[ "Catalogs" ][ "ItemTypes" ] = "ItemType.json";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Map.json" ).string(), mapJson );
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    void writeItemTypesJson( const Json::Value& itemTypesArray ) const {
        Json::Value itemTypeJson;
        itemTypeJson[ "ItemTypes" ] = itemTypesArray;
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "ItemType.json" ).string(), itemTypeJson );
    }

    QString configPath() const {
        return QString::fromStdString( _tempDir.string() ) + "/";
    }

    std::filesystem::path _tempDir;
};

} // namespace

TEST_F( ItemTypeFactoryTest, CreateItemTypeCatalog_ParsesFields ) {
    Json::Value itemType;
    itemType[ "Type" ] = 1;
    itemType[ "Name" ] = "Sword";

    Json::Value itemTypes( Json::arrayValue );
    itemTypes.append( itemType );
    writeItemTypesJson( itemTypes );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    ASSERT_NE( catalog.itemType( 1 ), nullptr );
    EXPECT_EQ( catalog.itemType( 1 )->name(), "Sword" );
}

TEST_F( ItemTypeFactoryTest, CreateItemTypeCatalog_MultipleItemTypes_AllAdded ) {
    Json::Value first;
    first[ "Type" ] = 1;
    first[ "Name" ] = "Sword";

    Json::Value second;
    second[ "Type" ] = 3;
    second[ "Name" ] = "Shield";

    Json::Value itemTypes( Json::arrayValue );
    itemTypes.append( first );
    itemTypes.append( second );
    writeItemTypesJson( itemTypes );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.itemTypes().size(), 2u );
}

TEST_F( ItemTypeFactoryTest, CreateItemTypeCatalog_MissingName_SkipsEntry ) {
    Json::Value invalid;
    invalid[ "Type" ] = 1;

    Json::Value valid;
    valid[ "Type" ] = 3;
    valid[ "Name" ] = "Shield";

    Json::Value itemTypes( Json::arrayValue );
    itemTypes.append( invalid );
    itemTypes.append( valid );
    writeItemTypesJson( itemTypes );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.itemType( 1 ), nullptr );
    ASSERT_NE( catalog.itemType( 3 ), nullptr );
    EXPECT_EQ( catalog.itemTypes().size(), 1u );
}

TEST_F( ItemTypeFactoryTest, SaveItemTypeCatalog_ThenCreateItemTypeCatalog_RoundTripsFields ) {
    Engine::ItemTypeCatalog original;
    Engine::ItemTypeModel itemType;
    itemType.setType( 1 );
    itemType.setName( "Sword" );
    original.addItemType( itemType );

    Engine::ItemTypeFactory::saveItemTypeCatalog( configPath(), original );

    Engine::ItemTypeCatalog reloaded;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), reloaded );

    ASSERT_NE( reloaded.itemType( 1 ), nullptr );
    EXPECT_EQ( reloaded.itemType( 1 )->name(), "Sword" );
}

TEST_F( ItemTypeFactoryTest, SaveItemTypeCatalog_PreservesUnknownFieldsLikeSkillTree ) {
    Json::Value itemType;
    itemType[ "Type" ] = 1;
    itemType[ "Name" ] = "Sword";
    Json::Value skillTree( Json::arrayValue );
    Json::Value node;
    node[ "Type" ] = 1;
    node[ "Name" ] = "Sword Mastery";
    skillTree.append( node );
    itemType[ "SkillTree" ] = skillTree;

    Json::Value itemTypes( Json::arrayValue );
    itemTypes.append( itemType );
    writeItemTypesJson( itemTypes );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeModel renamed;
    renamed.setType( 1 );
    renamed.setName( "Longsword" );
    catalog.addItemType( renamed );

    Engine::ItemTypeFactory::saveItemTypeCatalog( configPath(), catalog );

    const Json::Value savedJson = Engine::JsonHelper::loadJsonFile( ( _tempDir / "TestMap" / "ItemType.json" ).string() );

    ASSERT_EQ( savedJson[ "ItemTypes" ].size(), 1u );
    EXPECT_EQ( savedJson[ "ItemTypes" ][ 0 ][ "Name" ].asString(), "Longsword" );
    ASSERT_TRUE( savedJson[ "ItemTypes" ][ 0 ].isMember( "SkillTree" ) );
    EXPECT_EQ( savedJson[ "ItemTypes" ][ 0 ][ "SkillTree" ].size(), 1u );
}
