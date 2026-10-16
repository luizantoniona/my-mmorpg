#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Item/ItemTypeFactory.h>
#include <MMORPGEngine/Data/Skill/SkillFactory.h>

namespace {

class SkillFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_skillfactory";
        std::filesystem::create_directories( _tempDir / "TestMap" / "Items" );

        Json::Value configJson;
        configJson[ "ActiveFolder" ] = "TestMap";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "Config.json" ).string(), configJson );

        Json::Value mapJson;
        mapJson[ "Catalogs" ][ "Items" ] = "Items.json";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Map.json" ).string(), mapJson );

        Json::Value indexJson;
        indexJson[ "ItemTypes" ] = Json::Value( Json::arrayValue );
        indexJson[ "ItemTypes" ].append( "SWORD" );
        indexJson[ "ItemTypes" ].append( "GHOST" );
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Items.json" ).string(), indexJson );

        Engine::ItemTypeModel sword;
        sword.setType( "SWORD" );
        sword.setName( "Sword" );
        _itemTypeCatalog.addItemType( sword );
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    void writeItemTypeJson( const std::string& key, const Json::Value& itemTypeJson ) const {
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Items" / ( key + ".json" ) ).string(), itemTypeJson );
    }

    QString configPath() const {
        return QString::fromStdString( _tempDir.string() ) + "/";
    }

    std::filesystem::path _tempDir;
    Engine::ItemTypeCatalog _itemTypeCatalog;
};

class NoTreeCase {
public:
    NoTreeCase( std::string name, std::string key, std::string skillTree ) :
        name( std::move( name ) ),
        key( std::move( key ) ),
        skillTree( std::move( skillTree ) ) {
    }

    std::string name;
    std::string key;
    std::string skillTree;
};

class SkillNodeWrongTypeCase {
public:
    SkillNodeWrongTypeCase( std::string name, std::string key, Json::Value value ) :
        name( std::move( name ) ),
        key( std::move( key ) ),
        value( std::move( value ) ) {
    }

    std::string name;
    std::string key;
    Json::Value value;
};

Json::Value textArray() {
    Json::Value array( Json::arrayValue );
    array.append( "one" );

    return array;
}

class SkillFactoryNodeWrongTypeTest : public SkillFactoryTest, public ::testing::WithParamInterface<SkillNodeWrongTypeCase> {};
class SkillFactoryNoTreeTest : public SkillFactoryTest, public ::testing::WithParamInterface<NoTreeCase> {};

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
    itemType[ "Name" ] = "Sword";
    itemType[ "SkillTree" ] = nodes;
    writeItemTypeJson( "SWORD", itemType );

    Engine::SkillCatalog catalog;
    Engine::SkillFactory::createSkillCatalog( configPath(), _itemTypeCatalog, catalog );

    ASSERT_NE( catalog.tree( "SWORD" ), nullptr );
    ASSERT_NE( catalog.tree( "SWORD" )->node( 1 ), nullptr );

    const Engine::SkillNodeModel* skillNode = catalog.tree( "SWORD" )->node( 1 );
    EXPECT_EQ( skillNode->name(), "Sword Mastery" );
    EXPECT_EQ( skillNode->proficiencyLevel(), 5u );
    ASSERT_EQ( skillNode->prerequisites().size(), 1 );
    EXPECT_EQ( skillNode->prerequisites().first(), 7u );
}

TEST_P( SkillFactoryNoTreeTest, CreateSkillCatalog_TreeIsNotCreated ) {
    Json::Value itemType;
    itemType[ "Name" ] = "Item";

    if ( GetParam().skillTree == "emptyArray" ) {
        itemType[ "SkillTree" ] = Json::Value( Json::arrayValue );
    } else if ( GetParam().skillTree == "nodeWithoutType" ) {
        Json::Value node;
        node[ "Name" ] = "Sword Mastery";

        itemType[ "SkillTree" ] = Json::Value( Json::arrayValue );
        itemType[ "SkillTree" ].append( node );
    } else if ( GetParam().skillTree == "number" ) {
        itemType[ "SkillTree" ] = 5;
    } else if ( GetParam().skillTree == "text" ) {
        itemType[ "SkillTree" ] = "tree";
    } else if ( GetParam().skillTree == "object" ) {
        itemType[ "SkillTree" ] = Json::Value( Json::objectValue );
    } else if ( GetParam().skillTree == "null" ) {
        itemType[ "SkillTree" ] = Json::Value( Json::nullValue );
    }

    writeItemTypeJson( GetParam().key, itemType );

    Engine::SkillCatalog catalog;
    Engine::SkillFactory::createSkillCatalog( configPath(), _itemTypeCatalog, catalog );

    EXPECT_EQ( catalog.tree( QString::fromStdString( GetParam().key ) ), nullptr );
    EXPECT_EQ( catalog.trees().size(), 0u );
}

INSTANTIATE_TEST_SUITE_P(
    Trees,
    SkillFactoryNoTreeTest,
    ::testing::Values(
        NoTreeCase( "ItemTypeWithoutSkillTree_IsIgnored", "SWORD", "absent" ),
        NoTreeCase( "UnknownItemType_SkipsTree", "GHOST", "emptyArray" ),
        NoTreeCase( "NodeMissingType_SkipsTree", "SWORD", "nodeWithoutType" ),
        NoTreeCase( "SkillTreeIsNumber_SkipsTree", "SWORD", "number" ),
        NoTreeCase( "SkillTreeIsText_SkipsTree", "SWORD", "text" ),
        NoTreeCase( "SkillTreeIsObject_SkipsTree", "SWORD", "object" ),
        NoTreeCase( "SkillTreeIsNull_SkipsTree", "SWORD", "null" ) ),
    []( const ::testing::TestParamInfo<NoTreeCase>& info ) { return info.param.name; } );

TEST_P( SkillFactoryNodeWrongTypeTest, CreateSkillCatalog_NodeWithWrongFieldType_SkipsTree ) {
    Json::Value node;
    node[ "Type" ] = 1;
    node[ "Name" ] = "Sword Mastery";
    node[ GetParam().key ] = GetParam().value;

    Json::Value itemType;
    itemType[ "Name" ] = "Sword";
    itemType[ "SkillTree" ] = Json::Value( Json::arrayValue );
    itemType[ "SkillTree" ].append( node );
    writeItemTypeJson( "SWORD", itemType );

    Engine::SkillCatalog catalog;
    Engine::SkillFactory::createSkillCatalog( configPath(), _itemTypeCatalog, catalog );

    EXPECT_EQ( catalog.tree( "SWORD" ), nullptr );
}

INSTANTIATE_TEST_SUITE_P(
    Fields,
    SkillFactoryNodeWrongTypeTest,
    ::testing::Values(
        SkillNodeWrongTypeCase( "TypeIsText", "Type", Json::Value( "one" ) ),
        SkillNodeWrongTypeCase( "NameIsNumber", "Name", Json::Value( 5 ) ),
        SkillNodeWrongTypeCase( "ProficiencyLevelIsText", "ProficiencyLevel", Json::Value( "high" ) ),
        SkillNodeWrongTypeCase( "PrerequisitesIsNumber", "Prerequisites", Json::Value( 7 ) ),
        SkillNodeWrongTypeCase( "PrerequisiteEntryIsText", "Prerequisites", textArray() ) ),
    []( const ::testing::TestParamInfo<SkillNodeWrongTypeCase>& info ) { return info.param.name; } );

TEST_F( SkillFactoryTest, CreateSkillCatalog_NodeEntryNotAnObject_SkipsTree ) {
    Json::Value itemType;
    itemType[ "Name" ] = "Sword";
    itemType[ "SkillTree" ] = Json::Value( Json::arrayValue );
    itemType[ "SkillTree" ].append( 5 );
    writeItemTypeJson( "SWORD", itemType );

    Engine::SkillCatalog catalog;
    Engine::SkillFactory::createSkillCatalog( configPath(), _itemTypeCatalog, catalog );

    EXPECT_EQ( catalog.tree( "SWORD" ), nullptr );
}

TEST_F( SkillFactoryTest, CreateSkillCatalog_EmptySkillTree_IsValidAndCreatesEmptyTree ) {
    Json::Value itemType;
    itemType[ "Name" ] = "Sword";
    itemType[ "SkillTree" ] = Json::Value( Json::arrayValue );
    writeItemTypeJson( "SWORD", itemType );

    Engine::SkillCatalog catalog;
    Engine::SkillFactory::createSkillCatalog( configPath(), _itemTypeCatalog, catalog );

    ASSERT_NE( catalog.tree( "SWORD" ), nullptr );
    EXPECT_TRUE( catalog.tree( "SWORD" )->nodes().empty() );
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
    itemType[ "Name" ] = "Sword";
    itemType[ "SkillTree" ] = nodes;
    writeItemTypeJson( "SWORD", itemType );

    Engine::SkillCatalog catalog;
    Engine::SkillFactory::createSkillCatalog( configPath(), _itemTypeCatalog, catalog );

    ASSERT_NE( catalog.tree( "SWORD" ), nullptr );
    EXPECT_EQ( catalog.tree( "SWORD" )->nodes().size(), 2u );
}

TEST_F( SkillFactoryTest, SaveSkillCatalog_ThenCreateSkillCatalog_RoundTripsFields ) {
    Json::Value itemType;
    itemType[ "Name" ] = "Sword";
    writeItemTypeJson( "SWORD", itemType );

    Engine::SkillCatalog original;

    Engine::SkillTreeModel tree;
    tree.setItemType( "SWORD" );

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

    ASSERT_NE( reloaded.tree( "SWORD" ), nullptr );
    ASSERT_NE( reloaded.tree( "SWORD" )->node( 2 ), nullptr );
    EXPECT_EQ( reloaded.tree( "SWORD" )->node( 2 )->name(), "Whirlwind Strike" );
    EXPECT_EQ( reloaded.tree( "SWORD" )->node( 2 )->proficiencyLevel(), 10u );
    ASSERT_EQ( reloaded.tree( "SWORD" )->node( 2 )->prerequisites().size(), 1 );
    EXPECT_EQ( reloaded.tree( "SWORD" )->node( 2 )->prerequisites().first(), 1u );
}

TEST_F( SkillFactoryTest, SaveSkillCatalog_PreservesItemTypeFields ) {
    Json::Value itemType;
    itemType[ "Name" ] = "Sword";
    itemType[ "Category" ] = "WEAPON";
    itemType[ "Slot" ] = "HAND";
    writeItemTypeJson( "SWORD", itemType );

    Engine::SkillCatalog original;

    Engine::SkillTreeModel tree;
    tree.setItemType( "SWORD" );
    original.addTree( tree );

    Engine::SkillFactory::saveSkillCatalog( configPath(), original );

    Engine::ItemTypeCatalog reloadedItemTypeCatalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), reloadedItemTypeCatalog );

    ASSERT_NE( reloadedItemTypeCatalog.itemType( "SWORD" ), nullptr );
    EXPECT_EQ( reloadedItemTypeCatalog.itemType( "SWORD" )->name(), "Sword" );
}

TEST_F( SkillFactoryTest, SaveSkillCatalog_UnknownItemType_DoesNotCreateEntry ) {
    Json::Value itemType;
    itemType[ "Name" ] = "Sword";
    writeItemTypeJson( "SWORD", itemType );

    Engine::SkillCatalog original;

    Engine::SkillTreeModel tree;
    tree.setItemType( "UNLISTED" );
    original.addTree( tree );

    Engine::SkillFactory::saveSkillCatalog( configPath(), original );

    Engine::SkillCatalog reloaded;
    Engine::SkillFactory::createSkillCatalog( configPath(), _itemTypeCatalog, reloaded );

    EXPECT_EQ( reloaded.tree( "UNLISTED" ), nullptr );
}

TEST_F( SkillFactoryTest, SaveSkillCatalog_PreservesItemsOfTheType ) {
    Json::Value item;
    item[ "Id" ] = 1;
    item[ "Name" ] = "Iron Sword";

    Json::Value items( Json::arrayValue );
    items.append( item );

    Json::Value itemType;
    itemType[ "Name" ] = "Sword";
    itemType[ "Items" ] = items;
    writeItemTypeJson( "SWORD", itemType );

    Engine::SkillCatalog original;

    Engine::SkillTreeModel tree;
    tree.setItemType( "SWORD" );
    original.addTree( tree );

    Engine::SkillFactory::saveSkillCatalog( configPath(), original );

    const Json::Value saved = Engine::JsonHelper::loadJsonFile( ( _tempDir / "TestMap" / "Items" / "SWORD.json" ).string() );

    ASSERT_TRUE( saved.isMember( "Items" ) );
    EXPECT_EQ( saved[ "Items" ].size(), 1u );
}
