#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include <QImage>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Item/ItemFactory.h>

namespace {

class ItemFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_itemfactory";
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
        indexJson[ "ItemTypes" ].append( "SHIELD" );
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Items.json" ).string(), indexJson );

        writeItemsJson( "SWORD", Json::Value( Json::arrayValue ) );
        writeItemsJson( "SHIELD", Json::Value( Json::arrayValue ) );

        Engine::ItemTypeModel sword;
        sword.setType( "SWORD" );
        sword.setName( "Sword" );
        _itemTypeCatalog.addItemType( sword );

        Engine::ItemTypeModel shield;
        shield.setType( "SHIELD" );
        shield.setName( "Shield" );
        _itemTypeCatalog.addItemType( shield );
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    void writeItemsJson( const std::string& itemType, const Json::Value& itemsArray ) const {
        Json::Value itemTypeJson;
        itemTypeJson[ "Name" ] = itemType;
        itemTypeJson[ "Items" ] = itemsArray;
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Items" / ( itemType + ".json" ) ).string(), itemTypeJson );
    }

    static Json::Value itemJson( const std::string& id, const std::string& name ) {
        Json::Value json;
        json[ "Id" ] = id;
        json[ "Name" ] = name;
        return json;
    }

    QString configPath() const {
        return QString::fromStdString( _tempDir.string() ) + "/";
    }

    std::filesystem::path _tempDir;
    Engine::ItemTypeCatalog _itemTypeCatalog;
};

class InvalidItemCase {
public:
    InvalidItemCase( std::string name, bool includeId, Json::Value id, bool includeName, std::string itemName ) :
        name( std::move( name ) ),
        includeId( includeId ),
        id( std::move( id ) ),
        includeName( includeName ),
        itemName( std::move( itemName ) ) {
    }

    std::string name;
    bool includeId;
    Json::Value id;
    bool includeName;
    std::string itemName;
};

class ItemWrongTypeCase {
public:
    ItemWrongTypeCase( std::string name, std::string key, Json::Value value ) :
        name( std::move( name ) ),
        key( std::move( key ) ),
        value( std::move( value ) ) {
    }

    std::string name;
    std::string key;
    Json::Value value;
};

class ItemFactoryWrongTypeTest : public ItemFactoryTest, public ::testing::WithParamInterface<ItemWrongTypeCase> {};
class ItemFactoryInvalidEntryTest : public ItemFactoryTest, public ::testing::WithParamInterface<InvalidItemCase> {};

} // namespace

TEST_F( ItemFactoryTest, CreateItemCatalog_ParsesFields ) {
    Json::Value items( Json::arrayValue );
    items.append( itemJson( "IRON_SWORD", "Iron Sword" ) );
    writeItemsJson( "SWORD", items );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    ASSERT_NE( catalog.item( "IRON_SWORD" ), nullptr );
    EXPECT_EQ( catalog.item( "IRON_SWORD" )->name(), "Iron Sword" );
    EXPECT_EQ( catalog.item( "IRON_SWORD" )->idItemType(), "SWORD" );
}

TEST_F( ItemFactoryTest, CreateItemCatalog_ItemsOfDifferentTypes_AllAdded ) {
    Json::Value swords( Json::arrayValue );
    swords.append( itemJson( "IRON_SWORD", "Iron Sword" ) );
    swords.append( itemJson( "STEEL_SWORD", "Steel Sword" ) );
    writeItemsJson( "SWORD", swords );

    Json::Value shields( Json::arrayValue );
    shields.append( itemJson( "WOODEN_SHIELD", "Wooden Shield" ) );
    writeItemsJson( "SHIELD", shields );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    EXPECT_EQ( catalog.items().size(), 3u );
    ASSERT_NE( catalog.item( "WOODEN_SHIELD" ), nullptr );
    EXPECT_EQ( catalog.item( "WOODEN_SHIELD" )->idItemType(), "SHIELD" );
}

TEST_P( ItemFactoryInvalidEntryTest, CreateItemCatalog_InvalidEntry_IsSkippedAndOthersLoad ) {
    Json::Value invalid;
    if ( GetParam().includeId ) {
        invalid[ "Id" ] = GetParam().id;
    }
    if ( GetParam().includeName ) {
        invalid[ "Name" ] = GetParam().itemName;
    }

    Json::Value items( Json::arrayValue );
    items.append( invalid );
    items.append( itemJson( "STEEL_SWORD", "Steel Sword" ) );
    writeItemsJson( "SWORD", items );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    EXPECT_EQ( catalog.item( "BROKEN_ITEM" ), nullptr );
    ASSERT_NE( catalog.item( "STEEL_SWORD" ), nullptr );
    EXPECT_EQ( catalog.items().size(), 1u );
}

INSTANTIATE_TEST_SUITE_P(
    Entries,
    ItemFactoryInvalidEntryTest,
    ::testing::Values(
        InvalidItemCase( "MissingName", true, Json::Value( "BROKEN_ITEM" ), false, "" ),
        InvalidItemCase( "MissingId", false, Json::Value( "BROKEN_ITEM" ), true, "Ghost Sword" ),
        InvalidItemCase( "NumericId_IsRejected", true, Json::Value( 1 ), true, "Ghost Sword" ),
        InvalidItemCase( "NullId_IsRejected", true, Json::Value( Json::nullValue ), true, "Ghost Sword" ),
        InvalidItemCase( "EmptyId_IsRejected", true, Json::Value( "" ), true, "Ghost Sword" ),
        InvalidItemCase( "LowercaseId_IsRejected", true, Json::Value( "broken_item" ), true, "Ghost Sword" ),
        InvalidItemCase( "IdWithSpace_IsRejected", true, Json::Value( "BROKEN ITEM" ), true, "Ghost Sword" ),
        InvalidItemCase( "IdWithSlash_IsRejected", true, Json::Value( "BROKEN/ITEM" ), true, "Ghost Sword" ) ),
    []( const ::testing::TestParamInfo<InvalidItemCase>& info ) { return info.param.name; } );

TEST_P( ItemFactoryWrongTypeTest, CreateItemCatalog_WrongFieldType_SkipsEntryAndKeepsOthers ) {
    Json::Value invalid = itemJson( "BROKEN_ITEM", "Broken" );
    invalid[ GetParam().key ] = GetParam().value;

    Json::Value items( Json::arrayValue );
    items.append( invalid );
    items.append( itemJson( "STEEL_SWORD", "Steel Sword" ) );
    writeItemsJson( "SWORD", items );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    EXPECT_EQ( catalog.item( "BROKEN_ITEM" ), nullptr );
    EXPECT_NE( catalog.item( "STEEL_SWORD" ), nullptr );
    EXPECT_EQ( catalog.items().size(), 1u );
}

INSTANTIATE_TEST_SUITE_P(
    Fields,
    ItemFactoryWrongTypeTest,
    ::testing::Values(
        ItemWrongTypeCase( "NameIsNumber", "Name", Json::Value( 5 ) ),
        ItemWrongTypeCase( "TextureFolderIsNumber", "TextureFolder", Json::Value( 5 ) ),
        ItemWrongTypeCase( "IsAnimatedIsText", "IsAnimated", Json::Value( "yes" ) ),
        ItemWrongTypeCase( "FrameDurationIsText", "FrameDurationMs", Json::Value( "fast" ) ) ),
    []( const ::testing::TestParamInfo<ItemWrongTypeCase>& info ) { return info.param.name; } );

TEST_F( ItemFactoryTest, CreateItemCatalog_EntryNotAnObject_IsSkipped ) {
    Json::Value items( Json::arrayValue );
    items.append( 5 );
    items.append( itemJson( "STEEL_SWORD", "Steel Sword" ) );
    writeItemsJson( "SWORD", items );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    EXPECT_EQ( catalog.items().size(), 1u );
}

TEST_F( ItemFactoryTest, CreateItemCatalog_UnknownItemType_SkipsItems ) {
    Json::Value items( Json::arrayValue );
    items.append( itemJson( "GHOST_SWORD", "Ghost Sword" ) );
    writeItemsJson( "SWORD", items );

    Engine::ItemCatalog catalog;
    Engine::ItemTypeCatalog onlyShield;
    onlyShield.addItemType( *_itemTypeCatalog.itemType( "SHIELD" ) );
    Engine::ItemFactory::createItemCatalog( configPath(), onlyShield, catalog );

    EXPECT_EQ( catalog.item( "GHOST_SWORD" ), nullptr );
    EXPECT_EQ( catalog.items().size(), 0u );
}

TEST_F( ItemFactoryTest, CreateItemCatalog_DuplicatedId_KeepsFirst ) {
    Json::Value swords( Json::arrayValue );
    swords.append( itemJson( "IRON_SWORD", "Iron Sword" ) );
    writeItemsJson( "SWORD", swords );

    Json::Value shields( Json::arrayValue );
    shields.append( itemJson( "IRON_SWORD", "Wooden Shield" ) );
    writeItemsJson( "SHIELD", shields );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    ASSERT_NE( catalog.item( "IRON_SWORD" ), nullptr );
    EXPECT_EQ( catalog.item( "IRON_SWORD" )->name(), "Iron Sword" );
    EXPECT_EQ( catalog.items().size(), 1u );
}

TEST_F( ItemFactoryTest, SaveItemCatalog_ThenCreateItemCatalog_RoundTripsFields ) {
    Engine::ItemCatalog original;

    Engine::ItemModel sword;
    sword.setId( "IRON_SWORD" );
    sword.setIdItemType( "SWORD" );
    sword.setName( "Iron Sword" );
    original.addItem( sword );

    Engine::ItemModel shield;
    shield.setId( "WOODEN_SHIELD" );
    shield.setIdItemType( "SHIELD" );
    shield.setName( "Wooden Shield" );
    original.addItem( shield );

    Engine::ItemFactory::saveItemCatalog( configPath(), original );

    Engine::ItemCatalog reloaded;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, reloaded );

    ASSERT_NE( reloaded.item( "IRON_SWORD" ), nullptr );
    EXPECT_EQ( reloaded.item( "IRON_SWORD" )->name(), "Iron Sword" );
    EXPECT_EQ( reloaded.item( "IRON_SWORD" )->idItemType(), "SWORD" );
    ASSERT_NE( reloaded.item( "WOODEN_SHIELD" ), nullptr );
    EXPECT_EQ( reloaded.item( "WOODEN_SHIELD" )->idItemType(), "SHIELD" );
}

TEST_F( ItemFactoryTest, SaveItemCatalog_TypeWithoutItems_RemovesItemsButKeepsOtherFields ) {
    Json::Value items( Json::arrayValue );
    items.append( itemJson( "IRON_SWORD", "Iron Sword" ) );
    writeItemsJson( "SWORD", items );

    Engine::ItemFactory::saveItemCatalog( configPath(), Engine::ItemCatalog() );

    const Json::Value saved = Engine::JsonHelper::loadJsonFile( ( _tempDir / "TestMap" / "Items" / "SWORD.json" ).string() );

    EXPECT_FALSE( saved.isMember( "Items" ) );
    EXPECT_EQ( saved[ "Name" ].asString(), "SWORD" );
}

TEST_F( ItemFactoryTest, CreateItemCatalog_WithoutTextureFolder_LoadsWithoutAnimation ) {
    Json::Value items( Json::arrayValue );
    items.append( itemJson( "IRON_SWORD", "Iron Sword" ) );
    writeItemsJson( "SWORD", items );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    ASSERT_NE( catalog.item( "IRON_SWORD" ), nullptr );
    EXPECT_TRUE( catalog.item( "IRON_SWORD" )->folder().isEmpty() );
    EXPECT_TRUE( catalog.item( "IRON_SWORD" )->animation().isNull() );
}

TEST_F( ItemFactoryTest, CreateItemCatalog_MissingTextureFile_StillAddsItem ) {
    Json::Value item = itemJson( "IRON_SWORD", "Iron Sword" );
    item[ "TextureFolder" ] = "Textures/Items/Iron Sword";

    Json::Value items( Json::arrayValue );
    items.append( item );
    writeItemsJson( "SWORD", items );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    ASSERT_NE( catalog.item( "IRON_SWORD" ), nullptr );
    EXPECT_EQ( catalog.item( "IRON_SWORD" )->folder(), "Textures/Items/Iron Sword" );
    EXPECT_TRUE( catalog.item( "IRON_SWORD" )->animation().isNull() );
}

TEST_F( ItemFactoryTest, CreateItemCatalog_StaticTexture_LoadsAndRoundTrips ) {
    const std::filesystem::path textureDir = _tempDir / "TestMap" / "Textures" / "Items" / "Iron Sword";
    std::filesystem::create_directories( textureDir );

    QImage image( 32, 32, QImage::Format_ARGB32 );
    image.fill( Qt::red );
    ASSERT_TRUE( image.save( QString::fromStdString( ( textureDir / "Iron Sword.png" ).string() ) ) );

    Json::Value item = itemJson( "IRON_SWORD", "Iron Sword" );
    item[ "TextureFolder" ] = "Textures/Items/Iron Sword";

    Json::Value items( Json::arrayValue );
    items.append( item );
    writeItemsJson( "SWORD", items );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    ASSERT_NE( catalog.item( "IRON_SWORD" ), nullptr );
    EXPECT_FALSE( catalog.item( "IRON_SWORD" )->animation().isNull() );
    EXPECT_FALSE( catalog.item( "IRON_SWORD" )->animation().isAnimated() );
    EXPECT_EQ( catalog.item( "IRON_SWORD" )->texture().size(), QSize( 32, 32 ) );

    Engine::ItemFactory::saveItemCatalog( configPath(), catalog );

    Engine::ItemCatalog reloaded;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, reloaded );

    ASSERT_NE( reloaded.item( "IRON_SWORD" ), nullptr );
    EXPECT_EQ( reloaded.item( "IRON_SWORD" )->folder(), "Textures/Items/Iron Sword" );
    EXPECT_FALSE( reloaded.item( "IRON_SWORD" )->animation().isNull() );
}
