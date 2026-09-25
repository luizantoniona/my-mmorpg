#include "ItemTypeFactory.h"

#include <unordered_map>

#include <QDebug>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/DataFactory.h>
#include <MMORPGEngine/Data/Item/HandRequirementHelper.h>
#include <MMORPGEngine/Data/Item/ItemCategoryHelper.h>
#include <MMORPGEngine/Data/Item/ItemSlotHelper.h>
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

        if ( !itemTypeJson.isMember( "Type" ) || !itemTypeJson.isMember( "Name" ) ) {
            qWarning() << "ItemTypeFactory::createItemTypeCatalog"
                       << "Invalid ItemType, skipping";
            continue;
        }

        const ItemCategoryEnum category = itemTypeJson.isMember( "Category" ) ? ItemCategoryHelper::fromString( itemTypeJson[ "Category" ].asString() ) : ItemCategoryEnum::UNKNOWN;
        const ItemSlotEnum slot = itemTypeJson.isMember( "Slot" ) ? ItemSlotHelper::fromString( itemTypeJson[ "Slot" ].asString() ) : ItemSlotEnum::UNKNOWN;

        if ( category == ItemCategoryEnum::UNKNOWN || slot == ItemSlotEnum::UNKNOWN ) {
            qWarning() << "ItemTypeFactory::createItemTypeCatalog"
                       << "Invalid or unknown Category/Slot, skipping" << itemTypeJson[ "Type" ].asUInt();
            continue;
        }

        ItemTypeModel itemType;
        itemType.setType( itemTypeJson[ "Type" ].asUInt() );
        itemType.setName( QString( itemTypeJson[ "Name" ].asCString() ) );
        itemType.setCategory( category );
        itemType.setSlot( slot );

        if ( itemTypeJson.isMember( "HandRequirement" ) ) {
            const HandRequirementEnum handRequirement = HandRequirementHelper::fromString( itemTypeJson[ "HandRequirement" ].asString() );
            itemType.setHandRequirement( handRequirement );

            if ( handRequirement == HandRequirementEnum::UNKNOWN ) {
                qWarning() << "ItemTypeFactory::createItemTypeCatalog"
                           << "Unknown HandRequirement" << itemTypeJson[ "Type" ].asUInt();
            }
        }

        itemTypeCatalog.addItemType( std::move( itemType ) );
    }

    qInfo() << "ItemTypeFactory::createItemTypeCatalog";
}

void ItemTypeFactory::saveItemTypeCatalog( const QString& configPath, const ItemTypeCatalog& itemTypeCatalog ) {
    const QString path = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( path + "Map.json" );

    const QString itemTypesFile = path + QString( mapJson[ "Catalogs" ][ "ItemTypes" ].asCString() );

    const Json::Value existingJson = JsonHelper::loadJsonFile( itemTypesFile );

    std::unordered_map<uint32_t, Json::Value> existingByType;
    for ( const Json::Value& itemTypeJson : existingJson[ "ItemTypes" ] ) {
        if ( itemTypeJson.isMember( "Type" ) ) {
            existingByType[ itemTypeJson[ "Type" ].asUInt() ] = itemTypeJson;
        }
    }

    Json::Value json;
    json[ "ItemTypes" ] = Json::Value( Json::arrayValue );

    for ( const auto& entry : itemTypeCatalog.itemTypes() ) {
        const ItemTypeModel& itemType = entry.second;

        Json::Value itemTypeJson;
        const auto existing = existingByType.find( itemType.type() );
        if ( existing != existingByType.end() ) {
            itemTypeJson = existing->second;
        }

        itemTypeJson[ "Type" ] = itemType.type();
        itemTypeJson[ "Name" ] = itemType.name().toStdString();
        itemTypeJson[ "Category" ] = ItemCategoryHelper::toString( itemType.category() );
        itemTypeJson[ "Slot" ] = ItemSlotHelper::toString( itemType.slot() );

        if ( itemType.handRequirement().has_value() ) {
            itemTypeJson[ "HandRequirement" ] = HandRequirementHelper::toString( itemType.handRequirement().value() );
        } else {
            itemTypeJson.removeMember( "HandRequirement" );
        }

        json[ "ItemTypes" ].append( itemTypeJson );
    }

    qInfo() << "ItemTypeFactory::saveItemTypeCatalog"
            << "[ITEM_TYPES_FILE_PATH]" << itemTypesFile;

    JsonHelper::saveJsonFile( itemTypesFile, json );
}

} // namespace Engine
