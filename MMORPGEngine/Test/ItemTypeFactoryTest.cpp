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
    itemType[ "Category" ] = "WEAPON";
    itemType[ "Slot" ] = "Hand";
    itemType[ "HandRequirement" ] = "ONE_HANDED";

    Json::Value itemTypes( Json::arrayValue );
    itemTypes.append( itemType );
    writeItemTypesJson( itemTypes );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    ASSERT_NE( catalog.itemType( 1 ), nullptr );
    EXPECT_EQ( catalog.itemType( 1 )->name(), "Sword" );
    EXPECT_EQ( catalog.itemType( 1 )->category(), Engine::ItemCategoryEnum::WEAPON );
    EXPECT_EQ( catalog.itemType( 1 )->slot(), Engine::ItemSlotEnum::HAND );
    ASSERT_TRUE( catalog.itemType( 1 )->handRequirement().has_value() );
    EXPECT_EQ( catalog.itemType( 1 )->handRequirement().value(), Engine::HandRequirementEnum::ONE_HANDED );
}

TEST_F( ItemTypeFactoryTest, CreateItemTypeCatalog_MultipleItemTypes_AllAdded ) {
    Json::Value first;
    first[ "Type" ] = 1;
    first[ "Name" ] = "Sword";
    first[ "Category" ] = "WEAPON";
    first[ "Slot" ] = "Hand";

    Json::Value second;
    second[ "Type" ] = 3;
    second[ "Name" ] = "Shield";
    second[ "Category" ] = "SHIELD";
    second[ "Slot" ] = "Hand";

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
    valid[ "Category" ] = "SHIELD";
    valid[ "Slot" ] = "Hand";

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

TEST_F( ItemTypeFactoryTest, CreateItemTypeCatalog_MissingCategoryOrSlot_SkipsEntry ) {
    Json::Value missingCategory;
    missingCategory[ "Type" ] = 1;
    missingCategory[ "Name" ] = "Sword";
    missingCategory[ "Slot" ] = "Hand";

    Json::Value missingSlot;
    missingSlot[ "Type" ] = 2;
    missingSlot[ "Name" ] = "Axe";
    missingSlot[ "Category" ] = "WEAPON";

    Json::Value valid;
    valid[ "Type" ] = 3;
    valid[ "Name" ] = "Shield";
    valid[ "Category" ] = "SHIELD";
    valid[ "Slot" ] = "Hand";

    Json::Value itemTypes( Json::arrayValue );
    itemTypes.append( missingCategory );
    itemTypes.append( missingSlot );
    itemTypes.append( valid );
    writeItemTypesJson( itemTypes );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.itemType( 1 ), nullptr );
    EXPECT_EQ( catalog.itemType( 2 ), nullptr );
    ASSERT_NE( catalog.itemType( 3 ), nullptr );
    EXPECT_EQ( catalog.itemTypes().size(), 1u );
}

TEST_F( ItemTypeFactoryTest, CreateItemTypeCatalog_MissingHandRequirement_IsEmpty ) {
    Json::Value itemType;
    itemType[ "Type" ] = 17;
    itemType[ "Name" ] = "Helmet";
    itemType[ "Category" ] = "ARMOR";
    itemType[ "Slot" ] = "Head";

    Json::Value itemTypes( Json::arrayValue );
    itemTypes.append( itemType );
    writeItemTypesJson( itemTypes );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    ASSERT_NE( catalog.itemType( 17 ), nullptr );
    EXPECT_FALSE( catalog.itemType( 17 )->handRequirement().has_value() );
}

TEST_F( ItemTypeFactoryTest, CreateItemTypeCatalog_UnknownHandRequirement_IsFlaggedUnknown ) {
    Json::Value itemType;
    itemType[ "Type" ] = 1;
    itemType[ "Name" ] = "Sword";
    itemType[ "Category" ] = "WEAPON";
    itemType[ "Slot" ] = "Hand";
    itemType[ "HandRequirement" ] = "THREE_HANDED";

    Json::Value itemTypes( Json::arrayValue );
    itemTypes.append( itemType );
    writeItemTypesJson( itemTypes );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    ASSERT_NE( catalog.itemType( 1 ), nullptr );
    ASSERT_TRUE( catalog.itemType( 1 )->handRequirement().has_value() );
    EXPECT_EQ( catalog.itemType( 1 )->handRequirement().value(), Engine::HandRequirementEnum::UNKNOWN );
}

TEST_F( ItemTypeFactoryTest, SaveItemTypeCatalog_ThenCreateItemTypeCatalog_RoundTripsFields ) {
    Engine::ItemTypeCatalog original;
    Engine::ItemTypeModel itemType;
    itemType.setType( 1 );
    itemType.setName( "Sword" );
    itemType.setCategory( Engine::ItemCategoryEnum::WEAPON );
    itemType.setSlot( Engine::ItemSlotEnum::HAND );
    itemType.setHandRequirement( Engine::HandRequirementEnum::ONE_HANDED );
    original.addItemType( itemType );

    Engine::ItemTypeFactory::saveItemTypeCatalog( configPath(), original );

    Engine::ItemTypeCatalog reloaded;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), reloaded );

    ASSERT_NE( reloaded.itemType( 1 ), nullptr );
    EXPECT_EQ( reloaded.itemType( 1 )->name(), "Sword" );
    EXPECT_EQ( reloaded.itemType( 1 )->category(), Engine::ItemCategoryEnum::WEAPON );
    EXPECT_EQ( reloaded.itemType( 1 )->slot(), Engine::ItemSlotEnum::HAND );
    ASSERT_TRUE( reloaded.itemType( 1 )->handRequirement().has_value() );
    EXPECT_EQ( reloaded.itemType( 1 )->handRequirement().value(), Engine::HandRequirementEnum::ONE_HANDED );
}

TEST_F( ItemTypeFactoryTest, SaveItemTypeCatalog_PreservesUnknownFieldsLikeSkillTree ) {
    Json::Value itemType;
    itemType[ "Type" ] = 1;
    itemType[ "Name" ] = "Sword";
    itemType[ "Category" ] = "WEAPON";
    itemType[ "Slot" ] = "Hand";
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
    renamed.setCategory( Engine::ItemCategoryEnum::WEAPON );
    renamed.setSlot( Engine::ItemSlotEnum::HAND );
    catalog.addItemType( renamed );

    Engine::ItemTypeFactory::saveItemTypeCatalog( configPath(), catalog );

    const Json::Value savedJson = Engine::JsonHelper::loadJsonFile( ( _tempDir / "TestMap" / "ItemType.json" ).string() );

    ASSERT_EQ( savedJson[ "ItemTypes" ].size(), 1u );
    EXPECT_EQ( savedJson[ "ItemTypes" ][ 0 ][ "Name" ].asString(), "Longsword" );
    ASSERT_TRUE( savedJson[ "ItemTypes" ][ 0 ].isMember( "SkillTree" ) );
    EXPECT_EQ( savedJson[ "ItemTypes" ][ 0 ][ "SkillTree" ].size(), 1u );
}
