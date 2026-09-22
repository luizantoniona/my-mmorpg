#include <gtest/gtest.h>

#include <filesystem>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Item/ItemFactory.h>

namespace {

class ItemFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_itemfactory";
        std::filesystem::create_directories( _tempDir / "TestMap" );

        Json::Value configJson;
        configJson[ "ActiveFolder" ] = "TestMap";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "Config.json" ).string(), configJson );

        Json::Value mapJson;
        mapJson[ "Catalogs" ][ "Items" ] = "Item.json";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Map.json" ).string(), mapJson );

        Engine::ItemTypeModel itemType;
        itemType.setType( 1 );
        itemType.setName( "Sword" );
        _itemTypeCatalog.addItemType( itemType );
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    void writeItemsJson( const Json::Value& itemsArray ) const {
        Json::Value itemJson;
        itemJson[ "Items" ] = itemsArray;
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Item.json" ).string(), itemJson );
    }

    QString configPath() const {
        return QString::fromStdString( _tempDir.string() ) + "/";
    }

    std::filesystem::path _tempDir;
    Engine::ItemTypeCatalog _itemTypeCatalog;
};

} // namespace

TEST_F( ItemFactoryTest, CreateItemCatalog_ParsesFields ) {
    Json::Value item;
    item[ "Id" ] = 1;
    item[ "IdItemType" ] = 1;
    item[ "Name" ] = "Iron Sword";

    Json::Value items( Json::arrayValue );
    items.append( item );
    writeItemsJson( items );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    ASSERT_NE( catalog.item( 1 ), nullptr );
    EXPECT_EQ( catalog.item( 1 )->name(), "Iron Sword" );
    EXPECT_EQ( catalog.item( 1 )->idItemType(), 1u );
}

TEST_F( ItemFactoryTest, CreateItemCatalog_MultipleItems_AllAdded ) {
    Json::Value first;
    first[ "Id" ] = 1;
    first[ "IdItemType" ] = 1;
    first[ "Name" ] = "Iron Sword";

    Json::Value second;
    second[ "Id" ] = 2;
    second[ "IdItemType" ] = 1;
    second[ "Name" ] = "Steel Sword";

    Json::Value items( Json::arrayValue );
    items.append( first );
    items.append( second );
    writeItemsJson( items );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    EXPECT_EQ( catalog.items().size(), 2u );
}

TEST_F( ItemFactoryTest, CreateItemCatalog_MissingName_SkipsEntry ) {
    Json::Value invalid;
    invalid[ "Id" ] = 1;
    invalid[ "IdItemType" ] = 1;

    Json::Value valid;
    valid[ "Id" ] = 2;
    valid[ "IdItemType" ] = 1;
    valid[ "Name" ] = "Steel Sword";

    Json::Value items( Json::arrayValue );
    items.append( invalid );
    items.append( valid );
    writeItemsJson( items );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    EXPECT_EQ( catalog.item( 1 ), nullptr );
    ASSERT_NE( catalog.item( 2 ), nullptr );
    EXPECT_EQ( catalog.items().size(), 1u );
}

TEST_F( ItemFactoryTest, CreateItemCatalog_UnknownItemType_SkipsEntry ) {
    Json::Value item;
    item[ "Id" ] = 1;
    item[ "IdItemType" ] = 99;
    item[ "Name" ] = "Ghost Sword";

    Json::Value items( Json::arrayValue );
    items.append( item );
    writeItemsJson( items );

    Engine::ItemCatalog catalog;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, catalog );

    EXPECT_EQ( catalog.item( 1 ), nullptr );
    EXPECT_EQ( catalog.items().size(), 0u );
}

TEST_F( ItemFactoryTest, SaveItemCatalog_ThenCreateItemCatalog_RoundTripsFields ) {
    Engine::ItemCatalog original;
    Engine::ItemModel item;
    item.setId( 1 );
    item.setIdItemType( 1 );
    item.setName( "Iron Sword" );
    original.addItem( item );

    Engine::ItemFactory::saveItemCatalog( configPath(), original );

    Engine::ItemCatalog reloaded;
    Engine::ItemFactory::createItemCatalog( configPath(), _itemTypeCatalog, reloaded );

    ASSERT_NE( reloaded.item( 1 ), nullptr );
    EXPECT_EQ( reloaded.item( 1 )->name(), "Iron Sword" );
}
