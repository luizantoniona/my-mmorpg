#include <gtest/gtest.h>

#include <filesystem>

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

    static Json::Value itemJson( int id, const std::string& name ) {
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

} // namespace

TEST_F( ItemFactoryTest, CreateItemCatalog_ParsesFields ) {
    Json::Value items( Json::arrayValue );
    items.append( itemJson( 1, "Iron Sword" ) );
    writeItemsJson( "SWORD", items );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    ASSERT_NE( catalog.item( 1 ), nullptr );
    EXPECT_EQ( catalog.item( 1 )->name(), "Iron Sword" );
    EXPECT_EQ( catalog.item( 1 )->idItemType(), "SWORD" );
}

TEST_F( ItemFactoryTest, CreateItemCatalog_ItemsOfDifferentTypes_AllAdded ) {
    Json::Value swords( Json::arrayValue );
    swords.append( itemJson( 1, "Iron Sword" ) );
    swords.append( itemJson( 2, "Steel Sword" ) );
    writeItemsJson( "SWORD", swords );

    Json::Value shields( Json::arrayValue );
    shields.append( itemJson( 3, "Wooden Shield" ) );
    writeItemsJson( "SHIELD", shields );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    EXPECT_EQ( catalog.items().size(), 3u );
    ASSERT_NE( catalog.item( 3 ), nullptr );
    EXPECT_EQ( catalog.item( 3 )->idItemType(), "SHIELD" );
}

TEST_F( ItemFactoryTest, CreateItemCatalog_MissingName_SkipsEntry ) {
    Json::Value invalid;
    invalid[ "Id" ] = 1;

    Json::Value items( Json::arrayValue );
    items.append( invalid );
    items.append( itemJson( 2, "Steel Sword" ) );
    writeItemsJson( "SWORD", items );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    EXPECT_EQ( catalog.item( 1 ), nullptr );
    ASSERT_NE( catalog.item( 2 ), nullptr );
    EXPECT_EQ( catalog.items().size(), 1u );
}

TEST_F( ItemFactoryTest, CreateItemCatalog_UnknownItemType_SkipsItems ) {
    Json::Value items( Json::arrayValue );
    items.append( itemJson( 1, "Ghost Sword" ) );
    writeItemsJson( "SWORD", items );

    Engine::ItemCatalog catalog;
    Engine::ItemTypeCatalog onlyShield;
    onlyShield.addItemType( *_itemTypeCatalog.itemType( "SHIELD" ) );
    Engine::ItemFactory::createItemCatalog( configPath(), onlyShield, catalog );

    EXPECT_EQ( catalog.item( 1 ), nullptr );
    EXPECT_EQ( catalog.items().size(), 0u );
}

TEST_F( ItemFactoryTest, CreateItemCatalog_DuplicatedId_KeepsFirst ) {
    Json::Value swords( Json::arrayValue );
    swords.append( itemJson( 1, "Iron Sword" ) );
    writeItemsJson( "SWORD", swords );

    Json::Value shields( Json::arrayValue );
    shields.append( itemJson( 1, "Wooden Shield" ) );
    writeItemsJson( "SHIELD", shields );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    ASSERT_NE( catalog.item( 1 ), nullptr );
    EXPECT_EQ( catalog.item( 1 )->name(), "Iron Sword" );
    EXPECT_EQ( catalog.items().size(), 1u );
}

TEST_F( ItemFactoryTest, SaveItemCatalog_ThenCreateItemCatalog_RoundTripsFields ) {
    Engine::ItemCatalog original;

    Engine::ItemModel sword;
    sword.setId( 1 );
    sword.setIdItemType( "SWORD" );
    sword.setName( "Iron Sword" );
    original.addItem( sword );

    Engine::ItemModel shield;
    shield.setId( 2 );
    shield.setIdItemType( "SHIELD" );
    shield.setName( "Wooden Shield" );
    original.addItem( shield );

    Engine::ItemFactory::saveItemCatalog( configPath(), original );

    Engine::ItemCatalog reloaded;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, reloaded );

    ASSERT_NE( reloaded.item( 1 ), nullptr );
    EXPECT_EQ( reloaded.item( 1 )->name(), "Iron Sword" );
    EXPECT_EQ( reloaded.item( 1 )->idItemType(), "SWORD" );
    ASSERT_NE( reloaded.item( 2 ), nullptr );
    EXPECT_EQ( reloaded.item( 2 )->idItemType(), "SHIELD" );
}

TEST_F( ItemFactoryTest, SaveItemCatalog_TypeWithoutItems_RemovesItemsButKeepsOtherFields ) {
    Json::Value items( Json::arrayValue );
    items.append( itemJson( 1, "Iron Sword" ) );
    writeItemsJson( "SWORD", items );

    Engine::ItemFactory::saveItemCatalog( configPath(), Engine::ItemCatalog() );

    const Json::Value saved = Engine::JsonHelper::loadJsonFile( ( _tempDir / "TestMap" / "Items" / "SWORD.json" ).string() );

    EXPECT_FALSE( saved.isMember( "Items" ) );
    EXPECT_EQ( saved[ "Name" ].asString(), "SWORD" );
}

TEST_F( ItemFactoryTest, CreateItemCatalog_WithoutTextureFolder_LoadsWithoutAnimation ) {
    Json::Value items( Json::arrayValue );
    items.append( itemJson( 1, "Iron Sword" ) );
    writeItemsJson( "SWORD", items );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    ASSERT_NE( catalog.item( 1 ), nullptr );
    EXPECT_TRUE( catalog.item( 1 )->folder().isEmpty() );
    EXPECT_TRUE( catalog.item( 1 )->animation().isNull() );
}

TEST_F( ItemFactoryTest, CreateItemCatalog_MissingTextureFile_StillAddsItem ) {
    Json::Value item = itemJson( 1, "Iron Sword" );
    item[ "TextureFolder" ] = "Textures/Items/Iron Sword";

    Json::Value items( Json::arrayValue );
    items.append( item );
    writeItemsJson( "SWORD", items );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    ASSERT_NE( catalog.item( 1 ), nullptr );
    EXPECT_EQ( catalog.item( 1 )->folder(), "Textures/Items/Iron Sword" );
    EXPECT_TRUE( catalog.item( 1 )->animation().isNull() );
}

TEST_F( ItemFactoryTest, CreateItemCatalog_StaticTexture_LoadsAndRoundTrips ) {
    const std::filesystem::path textureDir = _tempDir / "TestMap" / "Textures" / "Items" / "Iron Sword";
    std::filesystem::create_directories( textureDir );

    QImage image( 32, 32, QImage::Format_ARGB32 );
    image.fill( Qt::red );
    ASSERT_TRUE( image.save( QString::fromStdString( ( textureDir / "Iron Sword.png" ).string() ) ) );

    Json::Value item = itemJson( 1, "Iron Sword" );
    item[ "TextureFolder" ] = "Textures/Items/Iron Sword";

    Json::Value items( Json::arrayValue );
    items.append( item );
    writeItemsJson( "SWORD", items );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    ASSERT_NE( catalog.item( 1 ), nullptr );
    EXPECT_FALSE( catalog.item( 1 )->animation().isNull() );
    EXPECT_FALSE( catalog.item( 1 )->animation().isAnimated() );
    EXPECT_EQ( catalog.item( 1 )->texture().size(), QSize( 32, 32 ) );

    Engine::ItemFactory::saveItemCatalog( configPath(), catalog );

    Engine::ItemCatalog reloaded;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, reloaded );

    ASSERT_NE( reloaded.item( 1 ), nullptr );
    EXPECT_EQ( reloaded.item( 1 )->folder(), "Textures/Items/Iron Sword" );
    EXPECT_FALSE( reloaded.item( 1 )->animation().isNull() );
}
