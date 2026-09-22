#include <gtest/gtest.h>

#include <filesystem>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Item/ItemTypeFactory.h>
#include <MMORPGEngine/Data/Skill/SkillFactory.h>

namespace {

class SkillFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_skillfactory";
        std::filesystem::create_directories( _tempDir / "TestMap" );

        Json::Value configJson;
        configJson[ "ActiveFolder" ] = "TestMap";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "Config.json" ).string(), configJson );

        Json::Value mapJson;
        mapJson[ "Catalogs" ][ "ItemTypes" ] = "ItemType.json";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Map.json" ).string(), mapJson );

        Engine::ItemTypeModel sword;
        sword.setType( 1 );
        sword.setName( "Sword" );
        _itemTypeCatalog.addItemType( sword );
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
    Engine::ItemTypeCatalog _itemTypeCatalog;
};

} // namespace

TEST_F( SkillFactoryTest, CreateSkillCatalog_ParsesFields ) {
    Json::Value node;
    node[ "Type" ] = 1;
    node[ "Name" ] = "Sword Mastery";
    node[ "ProficiencyLevel" ] = 5;
    Json::Value prerequisites( Json::arrayValue );
    prerequisites.append( 7 );
    node[ "Prerequisites" ] = prerequisites;

    Json::Value nodes( Json::arrayValue );
    nodes.append( node );

    Json::Value itemType;
    itemType[ "Type" ] = 1;
    itemType[ "Name" ] = "Sword";
    itemType[ "SkillTree" ] = nodes;

    Json::Value itemTypes( Json::arrayValue );
    itemTypes.append( itemType );
    writeItemTypesJson( itemTypes );

    Engine::SkillCatalog catalog;
    Engine::SkillFactory::createSkillCatalog( configPath(), _itemTypeCatalog, catalog );

    ASSERT_NE( catalog.tree( 1 ), nullptr );
    ASSERT_NE( catalog.tree( 1 )->node( 1 ), nullptr );

    const Engine::SkillNodeModel* skillNode = catalog.tree( 1 )->node( 1 );
    EXPECT_EQ( skillNode->name(), "Sword Mastery" );
    EXPECT_EQ( skillNode->proficiencyLevel(), 5u );
    ASSERT_EQ( skillNode->prerequisites().size(), 1 );
    EXPECT_EQ( skillNode->prerequisites().first(), 7u );
}

TEST_F( SkillFactoryTest, CreateSkillCatalog_ItemTypeWithoutSkillTree_IsIgnored ) {
    Json::Value itemType;
    itemType[ "Type" ] = 1;
    itemType[ "Name" ] = "Sword";

    Json::Value itemTypes( Json::arrayValue );
    itemTypes.append( itemType );
    writeItemTypesJson( itemTypes );

    Engine::SkillCatalog catalog;
    Engine::SkillFactory::createSkillCatalog( configPath(), _itemTypeCatalog, catalog );

    EXPECT_EQ( catalog.tree( 1 ), nullptr );
    EXPECT_EQ( catalog.trees().size(), 0u );
}

TEST_F( SkillFactoryTest, CreateSkillCatalog_UnknownItemType_SkipsTree ) {
    Json::Value itemType;
    itemType[ "Type" ] = 99;
    itemType[ "Name" ] = "Ghost";
    itemType[ "SkillTree" ] = Json::Value( Json::arrayValue );

    Json::Value itemTypes( Json::arrayValue );
    itemTypes.append( itemType );
    writeItemTypesJson( itemTypes );

    Engine::SkillCatalog catalog;
    Engine::SkillFactory::createSkillCatalog( configPath(), _itemTypeCatalog, catalog );

    EXPECT_EQ( catalog.tree( 99 ), nullptr );
    EXPECT_EQ( catalog.trees().size(), 0u );
}

TEST_F( SkillFactoryTest, CreateSkillCatalog_NodeMissingType_SkipsTree ) {
    Json::Value node;
    node[ "Name" ] = "Sword Mastery";

    Json::Value nodes( Json::arrayValue );
    nodes.append( node );

    Json::Value itemType;
    itemType[ "Type" ] = 1;
    itemType[ "Name" ] = "Sword";
    itemType[ "SkillTree" ] = nodes;

    Json::Value itemTypes( Json::arrayValue );
    itemTypes.append( itemType );
    writeItemTypesJson( itemTypes );

    Engine::SkillCatalog catalog;
    Engine::SkillFactory::createSkillCatalog( configPath(), _itemTypeCatalog, catalog );

    EXPECT_EQ( catalog.tree( 1 ), nullptr );
    EXPECT_EQ( catalog.trees().size(), 0u );
}

TEST_F( SkillFactoryTest, CreateSkillCatalog_MultipleNodesInTree_AllAdded ) {
    Json::Value first;
    first[ "Type" ] = 1;
    first[ "Name" ] = "Sword Mastery";

    Json::Value second;
    second[ "Type" ] = 2;
    second[ "Name" ] = "Whirlwind Strike";
    Json::Value prerequisites( Json::arrayValue );
    prerequisites.append( 1 );
    second[ "Prerequisites" ] = prerequisites;

    Json::Value nodes( Json::arrayValue );
    nodes.append( first );
    nodes.append( second );

    Json::Value itemType;
    itemType[ "Type" ] = 1;
    itemType[ "Name" ] = "Sword";
    itemType[ "SkillTree" ] = nodes;

    Json::Value itemTypes( Json::arrayValue );
    itemTypes.append( itemType );
    writeItemTypesJson( itemTypes );

    Engine::SkillCatalog catalog;
    Engine::SkillFactory::createSkillCatalog( configPath(), _itemTypeCatalog, catalog );

    ASSERT_NE( catalog.tree( 1 ), nullptr );
    EXPECT_EQ( catalog.tree( 1 )->nodes().size(), 2u );
}

TEST_F( SkillFactoryTest, SaveSkillCatalog_ThenCreateSkillCatalog_RoundTripsFields ) {
    Json::Value itemType;
    itemType[ "Type" ] = 1;
    itemType[ "Name" ] = "Sword";

    Json::Value itemTypes( Json::arrayValue );
    itemTypes.append( itemType );
    writeItemTypesJson( itemTypes );

    Engine::SkillCatalog original;

    Engine::SkillTreeModel tree;
    tree.setItemType( 1 );

    Engine::SkillNodeModel node;
    node.setType( 2 );
    node.setName( "Whirlwind Strike" );
    node.setProficiencyLevel( 10 );
    node.setPrerequisites( { 1 } );
    tree.addNode( node );

    original.addTree( tree );

    Engine::SkillFactory::saveSkillCatalog( configPath(), original );

    Engine::SkillCatalog reloaded;
    Engine::SkillFactory::createSkillCatalog( configPath(), _itemTypeCatalog, reloaded );

    ASSERT_NE( reloaded.tree( 1 ), nullptr );
    ASSERT_NE( reloaded.tree( 1 )->node( 2 ), nullptr );
    EXPECT_EQ( reloaded.tree( 1 )->node( 2 )->name(), "Whirlwind Strike" );
    EXPECT_EQ( reloaded.tree( 1 )->node( 2 )->proficiencyLevel(), 10u );
    ASSERT_EQ( reloaded.tree( 1 )->node( 2 )->prerequisites().size(), 1 );
    EXPECT_EQ( reloaded.tree( 1 )->node( 2 )->prerequisites().first(), 1u );
}

TEST_F( SkillFactoryTest, SaveSkillCatalog_PreservesItemTypeNameAndType ) {
    Json::Value itemType;
    itemType[ "Type" ] = 1;
    itemType[ "Name" ] = "Sword";

    Json::Value itemTypes( Json::arrayValue );
    itemTypes.append( itemType );
    writeItemTypesJson( itemTypes );

    Engine::SkillCatalog original;

    Engine::SkillTreeModel tree;
    tree.setItemType( 1 );
    original.addTree( tree );

    Engine::SkillFactory::saveSkillCatalog( configPath(), original );

    Engine::ItemTypeCatalog reloadedItemTypeCatalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), reloadedItemTypeCatalog );

    ASSERT_NE( reloadedItemTypeCatalog.itemType( 1 ), nullptr );
    EXPECT_EQ( reloadedItemTypeCatalog.itemType( 1 )->name(), "Sword" );
}

TEST_F( SkillFactoryTest, SaveSkillCatalog_UnknownItemType_DoesNotCreateEntry ) {
    Json::Value itemType;
    itemType[ "Type" ] = 1;
    itemType[ "Name" ] = "Sword";

    Json::Value itemTypes( Json::arrayValue );
    itemTypes.append( itemType );
    writeItemTypesJson( itemTypes );

    Engine::SkillCatalog original;

    Engine::SkillTreeModel tree;
    tree.setItemType( 99 );
    original.addTree( tree );

    Engine::SkillFactory::saveSkillCatalog( configPath(), original );

    Engine::SkillCatalog reloaded;
    Engine::SkillFactory::createSkillCatalog( configPath(), _itemTypeCatalog, reloaded );

    EXPECT_EQ( reloaded.tree( 99 ), nullptr );
}
