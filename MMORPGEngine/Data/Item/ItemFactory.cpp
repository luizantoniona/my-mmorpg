#include "ItemFactory.h"

#include <QDebug>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/DataFactory.h>
#include <MMORPGEngine/Data/Item/ItemModel.h>

namespace Engine {

void ItemFactory::createItemCatalog( const QString& configPath, const ItemTypeCatalog& itemTypeCatalog, ItemCatalog& itemCatalog ) {
    const QString mapPath = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    const QString itemsFile = mapPath + QString( mapJson[ "Catalogs" ][ "Items" ].asCString() );

    qInfo() << "ItemFactory::createItemCatalog" << "[ITEMS_FILE_PATH]" << itemsFile;

    Json::Value json = JsonHelper::loadJsonFile( itemsFile );

    const Json::Value& items = json[ "Items" ];

    for ( const Json::Value& itemJson : items ) {

        if ( !itemJson.isMember( "Id" ) || !itemJson.isMember( "IdItemType" ) || !itemJson.isMember( "Name" ) ) {
            qWarning() << "ItemFactory::createItemCatalog" << "Invalid Item, skipping";
            continue;
        }

        const uint32_t idItemType = itemJson[ "IdItemType" ].asUInt();
        if ( itemTypeCatalog.itemType( idItemType ) == nullptr ) {
            qWarning() << "ItemFactory::createItemCatalog" << "Unknown ItemType, skipping Item:" << itemJson[ "Id" ].asUInt();
            continue;
        }

        ItemModel item;
        item.setId( itemJson[ "Id" ].asUInt() );
        item.setIdItemType( idItemType );
        item.setName( QString( itemJson[ "Name" ].asCString() ) );

        itemCatalog.addItem( std::move( item ) );
    }

    qInfo() << "ItemFactory::createItemCatalog";
}

void ItemFactory::saveItemCatalog( const QString& configPath, const ItemCatalog& itemCatalog ) {
    const QString path = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( path + "Map.json" );

    const QString itemsFile = path + QString( mapJson[ "Catalogs" ][ "Items" ].asCString() );

    Json::Value json;
    json[ "Items" ] = Json::Value( Json::arrayValue );

    for ( const auto& entry : itemCatalog.items() ) {
        const ItemModel& item = entry.second;

        Json::Value itemJson;
        itemJson[ "Id" ] = item.id();
        itemJson[ "IdItemType" ] = item.idItemType();
        itemJson[ "Name" ] = item.name().toStdString();

        json[ "Items" ].append( itemJson );
    }

    qInfo() << "ItemFactory::saveItemCatalog" << "[ITEMS_FILE_PATH]" << itemsFile;

    JsonHelper::saveJsonFile( itemsFile, json );
}

} // namespace Engine
