#include <gtest/gtest.h>

#include <filesystem>
#include <optional>
#include <set>
#include <string>
#include <vector>

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

class InvalidItemTypeCase {
public:
    InvalidItemTypeCase( std::string name, std::string key, bool includeName, bool includeCategory, bool includeSlot ) :
        name( std::move( name ) ),
        key( std::move( key ) ),
        includeName( includeName ),
        includeCategory( includeCategory ),
        includeSlot( includeSlot ) {
    }

    std::string name;
    std::string key;
    bool includeName;
    bool includeCategory;
    bool includeSlot;
};

class HandRequirementCase {
public:
    HandRequirementCase( std::string name, bool present, std::string value, bool expectedPresent, Engine::HandRequirementEnum expected ) :
        name( std::move( name ) ),
        present( present ),
        value( std::move( value ) ),
        expectedPresent( expectedPresent ),
        expected( expected ) {
    }

    std::string name;
    bool present;
    std::string value;
    bool expectedPresent;
    Engine::HandRequirementEnum expected;
};

class DisabledActionsCase {
public:
    DisabledActionsCase( std::string name, bool present, std::vector<std::string> actions, std::set<Engine::CombatActionEnum> expected ) :
        name( std::move( name ) ),
        present( present ),
        actions( std::move( actions ) ),
        expected( std::move( expected ) ) {
    }

    std::string name;
    bool present;
    std::vector<std::string> actions;
    std::set<Engine::CombatActionEnum> expected;
};

class ItemTypeWrongTypeCase {
public:
    ItemTypeWrongTypeCase( std::string name, std::string key, Json::Value value ) :
        name( std::move( name ) ),
        key( std::move( key ) ),
        value( std::move( value ) ) {
    }

    std::string name;
    std::string key;
    Json::Value value;
};

class ItemTypeFactoryWrongTypeTest : public ItemTypeFactoryTest, public ::testing::WithParamInterface<ItemTypeWrongTypeCase> {};
class ItemTypeFactoryInvalidEntryTest : public ItemTypeFactoryTest, public ::testing::WithParamInterface<InvalidItemTypeCase> {};
class ItemTypeFactoryHandRequirementTest : public ItemTypeFactoryTest, public ::testing::WithParamInterface<HandRequirementCase> {};
class ItemTypeFactoryDisabledActionsTest : public ItemTypeFactoryTest, public ::testing::WithParamInterface<DisabledActionsCase> {};

} // namespace

TEST_F( ItemTypeFactoryTest, CreateItemTypeCatalog_ParsesFields ) {
    Json::Value json = itemTypeJson( "Sword", "WEAPON", "HAND" );
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
    writeItemType( "SWORD", itemTypeJson( "Sword", "WEAPON", "HAND" ) );
    writeItemType( "SHIELD", itemTypeJson( "Shield", "SHIELD", "HAND" ) );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.itemTypes().size(), 2u );
}

TEST_P( ItemTypeFactoryInvalidEntryTest, CreateItemTypeCatalog_InvalidEntry_IsSkippedAndOthersLoad ) {
    Json::Value invalid;
    if ( GetParam().includeName ) {
        invalid[ "Name" ] = "Invalid";
    }
    if ( GetParam().includeCategory ) {
        invalid[ "Category" ] = "WEAPON";
    }
    if ( GetParam().includeSlot ) {
        invalid[ "Slot" ] = "HAND";
    }
    writeItemType( GetParam().key, invalid );
    writeItemType( "SHIELD", itemTypeJson( "Shield", "SHIELD", "HAND" ) );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.itemType( QString::fromStdString( GetParam().key ) ), nullptr );
    ASSERT_NE( catalog.itemType( "SHIELD" ), nullptr );
    EXPECT_EQ( catalog.itemTypes().size(), 1u );
}

INSTANTIATE_TEST_SUITE_P(
    Entries,
    ItemTypeFactoryInvalidEntryTest,
    ::testing::Values(
        InvalidItemTypeCase( "MissingName", "SWORD", false, true, true ),
        InvalidItemTypeCase( "MissingCategory", "SWORD", true, false, true ),
        InvalidItemTypeCase( "MissingSlot", "AXE", true, true, false ),
        InvalidItemTypeCase( "LowercaseKey", "sword", true, true, true ) ),
    []( const ::testing::TestParamInfo<InvalidItemTypeCase>& info ) { return info.param.name; } );

TEST_P( ItemTypeFactoryWrongTypeTest, CreateItemTypeCatalog_WrongFieldType_SkipsEntryAndKeepsOthers ) {
    Json::Value invalid = itemTypeJson( "Sword", "WEAPON", "HAND" );
    invalid[ GetParam().key ] = GetParam().value;
    writeItemType( "SWORD", invalid );
    writeItemType( "SHIELD", itemTypeJson( "Shield", "SHIELD", "HAND" ) );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.itemType( "SWORD" ), nullptr );
    EXPECT_NE( catalog.itemType( "SHIELD" ), nullptr );
    EXPECT_EQ( catalog.itemTypes().size(), 1u );
}

INSTANTIATE_TEST_SUITE_P(
    Fields,
    ItemTypeFactoryWrongTypeTest,
    ::testing::Values(
        ItemTypeWrongTypeCase( "NameIsNumber", "Name", Json::Value( 5 ) ),
        ItemTypeWrongTypeCase( "CategoryIsNumber", "Category", Json::Value( 5 ) ),
        ItemTypeWrongTypeCase( "SlotIsNumber", "Slot", Json::Value( 5 ) ),
        ItemTypeWrongTypeCase( "HandRequirementIsNumber", "HandRequirement", Json::Value( 1 ) ),
        ItemTypeWrongTypeCase( "DisabledActionsIsText", "DisabledActions", Json::Value( "DODGE" ) ) ),
    []( const ::testing::TestParamInfo<ItemTypeWrongTypeCase>& info ) { return info.param.name; } );

TEST_P( ItemTypeFactoryHandRequirementTest, CreateItemTypeCatalog_HandRequirement ) {
    Json::Value json = itemTypeJson( "Item", "WEAPON", "HAND" );
    if ( GetParam().present ) {
        json[ "HandRequirement" ] = GetParam().value;
    }
    writeItemType( "ITEM", json );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    ASSERT_NE( catalog.itemType( "ITEM" ), nullptr );
    const std::optional<Engine::HandRequirementEnum>& handRequirement = catalog.itemType( "ITEM" )->handRequirement();

    ASSERT_EQ( handRequirement.has_value(), GetParam().expectedPresent );
    if ( GetParam().expectedPresent ) {
        EXPECT_EQ( handRequirement.value(), GetParam().expected );
    }
}

INSTANTIATE_TEST_SUITE_P(
    Values,
    ItemTypeFactoryHandRequirementTest,
    ::testing::Values(
        HandRequirementCase( "Missing_IsEmpty", false, "", false, Engine::HandRequirementEnum::UNKNOWN ),
        HandRequirementCase( "OneHanded", true, "ONE_HANDED", true, Engine::HandRequirementEnum::ONE_HANDED ),
        HandRequirementCase( "TwoHanded", true, "TWO_HANDED", true, Engine::HandRequirementEnum::TWO_HANDED ),
        HandRequirementCase( "UnknownName_IsFlaggedUnknown", true, "THREE_HANDED", true, Engine::HandRequirementEnum::UNKNOWN ),
        HandRequirementCase( "LowercaseName_IsFlaggedUnknown", true, "one_handed", true, Engine::HandRequirementEnum::UNKNOWN ) ),
    []( const ::testing::TestParamInfo<HandRequirementCase>& info ) { return info.param.name; } );

TEST_P( ItemTypeFactoryDisabledActionsTest, CreateItemTypeCatalog_DisabledActions ) {
    Json::Value json = itemTypeJson( "Shield", "SHIELD", "HAND" );
    if ( GetParam().present ) {
        json[ "DisabledActions" ] = Json::Value( Json::arrayValue );
        for ( const std::string& action : GetParam().actions ) {
            json[ "DisabledActions" ].append( action );
        }
    }
    writeItemType( "SHIELD", json );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    ASSERT_NE( catalog.itemType( "SHIELD" ), nullptr );
    EXPECT_EQ( catalog.itemType( "SHIELD" )->disabledActions(), GetParam().expected );
}

INSTANTIATE_TEST_SUITE_P(
    Values,
    ItemTypeFactoryDisabledActionsTest,
    ::testing::Values(
        DisabledActionsCase( "Missing_IsEmpty", false, {}, {} ),
        DisabledActionsCase( "EmptyList_IsEmpty", true, {}, {} ),
        DisabledActionsCase( "Dodge", true, { "DODGE" }, { Engine::CombatActionEnum::DODGE } ),
        DisabledActionsCase( "DodgeAndBlock", true, { "DODGE", "BLOCK" }, { Engine::CombatActionEnum::DODGE, Engine::CombatActionEnum::BLOCK } ),
        DisabledActionsCase( "UnknownAction_IsSkipped", true, { "FLY", "DODGE" }, { Engine::CombatActionEnum::DODGE } ),
        DisabledActionsCase( "OnlyUnknownActions_IsEmpty", true, { "FLY" }, {} ),
        DisabledActionsCase( "DuplicatedAction_IsDeduplicated", true, { "DODGE", "DODGE" }, { Engine::CombatActionEnum::DODGE } ) ),
    []( const ::testing::TestParamInfo<DisabledActionsCase>& info ) { return info.param.name; } );

TEST_F( ItemTypeFactoryTest, SaveItemTypeCatalog_RoundTripsDisabledActions ) {
    Engine::ItemTypeCatalog original;
    Engine::ItemTypeModel shield;
    shield.setType( "SHIELD" );
    shield.setName( "Shield" );
    shield.setCategory( Engine::ItemCategoryEnum::SHIELD );
    shield.setSlot( Engine::ItemSlotEnum::HAND );
    shield.setDisabledActions( { Engine::CombatActionEnum::DODGE } );
    original.addItemType( shield );

    Engine::ItemTypeFactory::saveItemTypeCatalog( configPath(), original );

    Engine::ItemTypeCatalog reloaded;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), reloaded );

    ASSERT_NE( reloaded.itemType( "SHIELD" ), nullptr );
    EXPECT_EQ( reloaded.itemType( "SHIELD" )->disabledActions(), std::set<Engine::CombatActionEnum>( { Engine::CombatActionEnum::DODGE } ) );
}

TEST_F( ItemTypeFactoryTest, SaveItemTypeCatalog_EmptyDisabledActions_RemovesField ) {
    Json::Value json = itemTypeJson( "Shield", "SHIELD", "HAND" );
    json[ "DisabledActions" ] = Json::Value( Json::arrayValue );
    json[ "DisabledActions" ].append( "DODGE" );
    writeItemType( "SHIELD", json );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeModel shield;
    shield.setType( "SHIELD" );
    shield.setName( "Shield" );
    shield.setCategory( Engine::ItemCategoryEnum::SHIELD );
    shield.setSlot( Engine::ItemSlotEnum::HAND );
    catalog.addItemType( shield );

    Engine::ItemTypeFactory::saveItemTypeCatalog( configPath(), catalog );

    const Json::Value savedJson = Engine::JsonHelper::loadJsonFile( ( _tempDir / "TestMap" / "Items" / "SHIELD.json" ).string() );

    EXPECT_FALSE( savedJson.isMember( "DisabledActions" ) );
}

TEST_F( ItemTypeFactoryTest, CreateItemTypeCatalog_IndexedTypeWithoutFile_IsSkipped ) {
    indexType( "SWORD" );
    writeItemType( "SHIELD", itemTypeJson( "Shield", "SHIELD", "HAND" ) );

    Engine::ItemTypeCatalog catalog;
    Engine::ItemTypeFactory::createItemTypeCatalog( configPath(), catalog );

    EXPECT_EQ( catalog.itemType( "SWORD" ), nullptr );
    ASSERT_NE( catalog.itemType( "SHIELD" ), nullptr );
}

TEST_F( ItemTypeFactoryTest, ReadTypeKeys_DuplicatedKey_KeepsFirst ) {
    writeItemType( "SWORD", itemTypeJson( "Sword", "WEAPON", "HAND" ) );
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
    Json::Value json = itemTypeJson( "Sword", "WEAPON", "HAND" );
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
    writeItemType( "SWORD", itemTypeJson( "Sword", "WEAPON", "HAND" ) );
    writeItemType( "SHIELD", itemTypeJson( "Shield", "SHIELD", "HAND" ) );

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
