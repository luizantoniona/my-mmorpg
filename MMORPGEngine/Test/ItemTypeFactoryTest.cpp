#include <gtest/gtest.h>

#include <filesystem>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Item/HandRequirementEnum.h>
#include <MMORPGEngine/Data/Item/ItemCategoryEnum.h>
#include <MMORPGEngine/Data/Item/ItemSlotEnum.h>
#include <MMORPGEngine/Data/Item/ItemTypeFactory.h>

namespace {

class ItemTypeFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_itemtypefactory";
        std::filesystem::create_directories( _tempDir / "TestMap" / "Items" );

        Json::Value configJson;
        configJson[ "ActiveFolder" ] = "TestMap";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "Config.json" ).string(), configJson );

        Json::Value mapJson;
        mapJson[ "Catalogs" ][ "Items" ] = "Items.json";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Map.json" ).string(), mapJson );

        _index = Json::Value( Json::arrayValue );
        writeIndex();
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    void writeIndex() const {
        Json::Value indexJson;
        indexJson[ "ItemTypes" ] = _index;
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Items.json" ).string(), indexJson );
    }

    void indexType( const std::string& key ) {
        _index.append( key );
        writeIndex();
    }

    void writeItemType( const std::string& key, const Json::Value& itemTypeJson ) {
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Items" / ( key + ".json" ) ).string(), itemTypeJson );
        indexType( key );
    }

    static Json::Value itemTypeJson( const std::string& name, const std::string& category, const std::string& slot ) {
        Json::Value json;
        json[ "Name" ] = name;
        json[ "Category" ] = category;
        json[ "Slot" ] = slot;
        return json;
    }

    QString configPath() const {
        return QString::fromStdString( _tempDir.string() ) + "/";
    }

    std::filesystem::path _tempDir;
    Json::Value _index;
};

} // namespace

TEST_F( ItemTypeFactoryTest, CreateItemTypeCatalog_ParsesFields ) {
    Json::Value json = itemTypeJson( "Sword", "WEAPON", "Hand" );
    json[ "HandRequirement" ] = "ONE_HANDED";
    writeItemType( "SWORD", json );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    ASSERT_NE( catalog.itemType( "SWORD" ), nullptr );
    EXPECT_EQ( catalog.itemType( "SWORD" )->type(), "SWORD" );
    EXPECT_EQ( catalog.itemType( "SWORD" )->name(), "Sword" );
    EXPECT_EQ( catalog.itemType( "SWORD" )->category(), Engine::ItemCategoryEnum::WEAPON );
    EXPECT_EQ( catalog.itemType( "SWORD" )->slot(), Engine::ItemSlotEnum::HAND );
    ASSERT_TRUE( catalog.itemType( "SWORD" )->handRequirement().has_value() );
    EXPECT_EQ( catalog.itemType( "SWORD" )->handRequirement().value(), Engine::HandRequirementEnum::ONE_HANDED );
}

TEST_F( ItemTypeFactoryTest, CreateItemTypeCatalog_MultipleItemTypes_AllAdded ) {
    writeItemType( "SWORD", itemTypeJson( "Sword", "WEAPON", "Hand" ) );
    writeItemType( "SHIELD", itemTypeJson( "Shield", "SHIELD", "Hand" ) );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.itemTypes().size(), 2u );
}

TEST_F( ItemTypeFactoryTest, CreateItemTypeCatalog_MissingName_SkipsEntry ) {
    Json::Value invalid;
    invalid[ "Category" ] = "WEAPON";
    invalid[ "Slot" ] = "Hand";
    writeItemType( "SWORD", invalid );
    writeItemType( "SHIELD", itemTypeJson( "Shield", "SHIELD", "Hand" ) );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.itemType( "SWORD" ), nullptr );
    ASSERT_NE( catalog.itemType( "SHIELD" ), nullptr );
    EXPECT_EQ( catalog.itemTypes().size(), 1u );
}

TEST_F( ItemTypeFactoryTest, CreateItemTypeCatalog_MissingCategoryOrSlot_SkipsEntry ) {
    Json::Value missingCategory;
    missingCategory[ "Name" ] = "Sword";
    missingCategory[ "Slot" ] = "Hand";
    writeItemType( "SWORD", missingCategory );

    Json::Value missingSlot;
    missingSlot[ "Name" ] = "Axe";
    missingSlot[ "Category" ] = "WEAPON";
    writeItemType( "AXE", missingSlot );

    writeItemType( "SHIELD", itemTypeJson( "Shield", "SHIELD", "Hand" ) );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.itemType( "SWORD" ), nullptr );
    EXPECT_EQ( catalog.itemType( "AXE" ), nullptr );
    ASSERT_NE( catalog.itemType( "SHIELD" ), nullptr );
    EXPECT_EQ( catalog.itemTypes().size(), 1u );
}

TEST_F( ItemTypeFactoryTest, CreateItemTypeCatalog_MissingHandRequirement_IsEmpty ) {
    writeItemType( "HELMET", itemTypeJson( "Helmet", "ARMOR", "Head" ) );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    ASSERT_NE( catalog.itemType( "HELMET" ), nullptr );
    EXPECT_FALSE( catalog.itemType( "HELMET" )->handRequirement().has_value() );
}

TEST_F( ItemTypeFactoryTest, CreateItemTypeCatalog_UnknownHandRequirement_IsFlaggedUnknown ) {
    Json::Value json = itemTypeJson( "Sword", "WEAPON", "Hand" );
    json[ "HandRequirement" ] = "THREE_HANDED";
    writeItemType( "SWORD", json );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    ASSERT_NE( catalog.itemType( "SWORD" ), nullptr );
    ASSERT_TRUE( catalog.itemType( "SWORD" )->handRequirement().has_value() );
    EXPECT_EQ( catalog.itemType( "SWORD" )->handRequirement().value(), Engine::HandRequirementEnum::UNKNOWN );
}

TEST_F( ItemTypeFactoryTest, CreateItemTypeCatalog_IndexedTypeWithoutFile_IsSkipped ) {
    indexType( "SWORD" );
    writeItemType( "SHIELD", itemTypeJson( "Shield", "SHIELD", "Hand" ) );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.itemType( "SWORD" ), nullptr );
    ASSERT_NE( catalog.itemType( "SHIELD" ), nullptr );
}

TEST_F( ItemTypeFactoryTest, CreateItemTypeCatalog_InvalidKey_IsSkipped ) {
    writeItemType( "sword", itemTypeJson( "Sword", "WEAPON", "Hand" ) );
    writeItemType( "SHIELD", itemTypeJson( "Shield", "SHIELD", "Hand" ) );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.itemType( "sword" ), nullptr );
    EXPECT_EQ( catalog.itemTypes().size(), 1u );
}

TEST_F( ItemTypeFactoryTest, ReadTypeKeys_DuplicatedKey_KeepsFirst ) {
    writeItemType( "SWORD", itemTypeJson( "Sword", "WEAPON", "Hand" ) );
    indexType( "SWORD" );

    const QStringList keys = Engine::ItemTypeFactory::readTypeKeys( configPath() );

    EXPECT_EQ( keys.size(), 1 );
}

TEST_F( ItemTypeFactoryTest, SaveItemTypeCatalog_ThenCreateItemTypeCatalog_RoundTripsFields ) {
    Engine::ItemTypeCatalog original;
    Engine::ItemTypeModel itemType;
    itemType.setType( "SWORD" );
    itemType.setName( "Sword" );
    itemType.setCategory( Engine::ItemCategoryEnum::WEAPON );
    itemType.setSlot( Engine::ItemSlotEnum::HAND );
    itemType.setHandRequirement( Engine::HandRequirementEnum::ONE_HANDED );
    original.addItemType( itemType );

    Engine::ItemTypeFactory::saveItemTypeCatalog( configPath(), original );

    Engine::ItemTypeCatalog reloaded;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), reloaded );

    ASSERT_NE( reloaded.itemType( "SWORD" ), nullptr );
    EXPECT_EQ( reloaded.itemType( "SWORD" )->name(), "Sword" );
    EXPECT_EQ( reloaded.itemType( "SWORD" )->category(), Engine::ItemCategoryEnum::WEAPON );
    EXPECT_EQ( reloaded.itemType( "SWORD" )->slot(), Engine::ItemSlotEnum::HAND );
    ASSERT_TRUE( reloaded.itemType( "SWORD" )->handRequirement().has_value() );
    EXPECT_EQ( reloaded.itemType( "SWORD" )->handRequirement().value(), Engine::HandRequirementEnum::ONE_HANDED );
}

TEST_F( ItemTypeFactoryTest, SaveItemTypeCatalog_PreservesUnknownFieldsLikeSkillTree ) {
    Json::Value json = itemTypeJson( "Sword", "WEAPON", "Hand" );
    Json::Value skillTree( Json::arrayValue );
    Json::Value node;
    node[ "Type" ] = 1;
    node[ "Name" ] = "Sword Mastery";
    skillTree.append( node );
    json[ "SkillTree" ] = skillTree;
    writeItemType( "SWORD", json );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeModel renamed;
    renamed.setType( "SWORD" );
    renamed.setName( "Longsword" );
    renamed.setCategory( Engine::ItemCategoryEnum::WEAPON );
    renamed.setSlot( Engine::ItemSlotEnum::HAND );
    catalog.addItemType( renamed );

    Engine::ItemTypeFactory::saveItemTypeCatalog( configPath(), catalog );

    const Json::Value savedJson = Engine::JsonHelper::loadJsonFile( ( _tempDir / "TestMap" / "Items" / "SWORD.json" ).string() );

    EXPECT_EQ( savedJson[ "Name" ].asString(), "Longsword" );
    ASSERT_TRUE( savedJson.isMember( "SkillTree" ) );
    EXPECT_EQ( savedJson[ "SkillTree" ].size(), 1u );
}

TEST_F( ItemTypeFactoryTest, SaveItemTypeCatalog_KeepsIndexOrderAndAppendsNewTypes ) {
    writeItemType( "SWORD", itemTypeJson( "Sword", "WEAPON", "Hand" ) );
    writeItemType( "SHIELD", itemTypeJson( "Shield", "SHIELD", "Hand" ) );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    Engine::ItemTypeModel axe;
    axe.setType( "AXE" );
    axe.setName( "Axe" );
    axe.setCategory( Engine::ItemCategoryEnum::WEAPON );
    axe.setSlot( Engine::ItemSlotEnum::HAND );
    catalog.addItemType( axe );

    Engine::ItemTypeFactory::saveItemTypeCatalog( configPath(), catalog );

    const QStringList keys = Engine::ItemTypeFactory::readTypeKeys( configPath() );

    ASSERT_EQ( keys.size(), 3 );
    EXPECT_EQ( keys[ 0 ], "SWORD" );
    EXPECT_EQ( keys[ 1 ], "SHIELD" );
    EXPECT_EQ( keys[ 2 ], "AXE" );
    EXPECT_TRUE( std::filesystem::exists( _tempDir / "TestMap" / "Items" / "AXE.json" ) );
}
