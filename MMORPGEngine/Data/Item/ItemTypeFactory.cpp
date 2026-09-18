#include "ItemTypeFactory.h"

#include <QDebug>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/DataFactory.h>
#include <MMORPGEngine/Data/Item/ItemTypeModel.h>

namespace Engine {

void ItemTypeFactory::createItemTypeCatalog( const QString& configPath, ItemTypeCatalog& itemTypeCatalog ) {
    const QString mapPath = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    const QString itemTypesFile = mapPath + QString( mapJson[ "Catalogs" ][ "ItemTypes" ].asCString() );

    qInfo() << "ItemTypeFactory::createItemTypeCatalog"
            << "[ITEM_TYPES_FILE_PATH]" << itemTypesFile;

    Json::Value json = JsonHelper::loadJsonFile( itemTypesFile );

    const Json::Value& itemTypes = json[ "ItemTypes" ];

    for ( const Json::Value& itemTypeJson : itemTypes ) {

        ItemTypeModel itemType;
        itemType.setType( itemTypeJson[ "Type" ].asUInt() );
        itemType.setName( QString( itemTypeJson[ "Name" ].asCString() ) );

        itemTypeCatalog.addItemType( std::move( itemType ) );
    }

    qInfo() << "ItemTypeFactory::createItemTypeCatalog";
}

void ItemTypeFactory::saveItemTypeCatalog( const QString& configPath, const ItemTypeCatalog& itemTypeCatalog ) {
    const QString path = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( path + "Map.json" );

    const QString itemTypesFile = path + QString( mapJson[ "Catalogs" ][ "ItemTypes" ].asCString() );

    Json::Value json;
    json[ "ItemTypes" ] = Json::Value( Json::arrayValue );

    for ( const auto& entry : itemTypeCatalog.itemTypes() ) {
        const ItemTypeModel& itemType = entry.second;

        Json::Value itemTypeJson;
        itemTypeJson[ "Type" ] = itemType.type();
        itemTypeJson[ "Name" ] = itemType.name().toStdString();

        json[ "ItemTypes" ].append( itemTypeJson );
    }

    qInfo() << "ItemTypeFactory::saveItemTypeCatalog"
            << "[ITEM_TYPES_FILE_PATH]" << itemTypesFile;

    JsonHelper::saveJsonFile( itemTypesFile, json );
}

} // namespace Engine
