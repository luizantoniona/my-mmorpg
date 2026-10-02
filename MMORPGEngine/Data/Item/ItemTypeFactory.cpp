#include "ItemTypeFactory.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QRegularExpression>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/DataFactory.h>
#include <MMORPGEngine/Data/Item/HandRequirementHelper.h>
#include <MMORPGEngine/Data/Item/ItemCategoryHelper.h>
#include <MMORPGEngine/Data/Item/ItemSlotHelper.h>
#include <MMORPGEngine/Data/Item/ItemTypeModel.h>

namespace Engine {

namespace {

QString indexFilePath( const QString& mapPath ) {
    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    return mapPath + QString( mapJson[ "Catalogs" ][ "Items" ].asCString() );
}

} // namespace

bool ItemTypeFactory::isValidType( const QString& type ) {
    static const QRegularExpression pattern( "^[A-Z0-9_]+$" );

    return pattern.match( type ).hasMatch();
}

QStringList ItemTypeFactory::readTypeKeys( const QString& configPath ) {
    const QString mapPath = DataFactory::mapPath( configPath );

    const Json::Value json = JsonHelper::loadJsonFile( indexFilePath( mapPath ) );

    QStringList keys;

    for ( const Json::Value& keyJson : json[ "ItemTypes" ] ) {
        if ( !keyJson.isString() ) {
            qWarning() << "ItemTypeFactory::readTypeKeys"
                       << "Invalid ItemType key, skipping";
            continue;
        }

        const QString key = QString::fromStdString( keyJson.asString() );

        if ( !isValidType( key ) ) {
            qWarning() << "ItemTypeFactory::readTypeKeys"
                       << "Invalid ItemType key (use A-Z, 0-9, _), skipping:" << key;
            continue;
        }

        if ( keys.contains( key ) ) {
            qWarning() << "ItemTypeFactory::readTypeKeys"
                       << "Duplicated ItemType key, skipping:" << key;
            continue;
        }

        keys.append( key );
    }

    return keys;
}

QString ItemTypeFactory::typeFilePath( const QString& configPath, const QString& type ) {
    return DataFactory::mapPath( configPath ) + "Items/" + type + ".json";
}

void ItemTypeFactory::createItemTypeCatalog( const QString& configPath, ItemTypeCatalog& itemTypeCatalog ) {
    qInfo() << "ItemTypeFactory::createItemTypeCatalog"
            << "[ITEM_TYPES_FOLDER_PATH]" << DataFactory::mapPath( configPath ) + "Items/";

    for ( const QString& key : readTypeKeys( configPath ) ) {
        const QString file = typeFilePath( configPath, key );

        if ( !QFile::exists( file ) ) {
            qWarning() << "ItemTypeFactory::createItemTypeCatalog"
                       << "ItemType file not found, skipping:" << file;
            continue;
        }

        const Json::Value itemTypeJson = JsonHelper::loadJsonFile( file );

        if ( !itemTypeJson.isMember( "Name" ) ) {
            qWarning() << "ItemTypeFactory::createItemTypeCatalog"
                       << "Invalid ItemType, skipping" << key;
            continue;
        }

        const ItemCategoryEnum category = itemTypeJson.isMember( "Category" ) ? ItemCategoryHelper::fromString( itemTypeJson[ "Category" ].asString() ) : ItemCategoryEnum::UNKNOWN;
        const ItemSlotEnum slot = itemTypeJson.isMember( "Slot" ) ? ItemSlotHelper::fromString( itemTypeJson[ "Slot" ].asString() ) : ItemSlotEnum::UNKNOWN;

        if ( category == ItemCategoryEnum::UNKNOWN || slot == ItemSlotEnum::UNKNOWN ) {
            qWarning() << "ItemTypeFactory::createItemTypeCatalog"
                       << "Invalid or unknown Category/Slot, skipping" << key;
            continue;
        }

        ItemTypeModel itemType;
        itemType.setType( key );
        itemType.setName( QString( itemTypeJson[ "Name" ].asCString() ) );
        itemType.setCategory( category );
        itemType.setSlot( slot );

        if ( itemTypeJson.isMember( "HandRequirement" ) ) {
            const HandRequirementEnum handRequirement = HandRequirementHelper::fromString( itemTypeJson[ "HandRequirement" ].asString() );
            itemType.setHandRequirement( handRequirement );

            if ( handRequirement == HandRequirementEnum::UNKNOWN ) {
                qWarning() << "ItemTypeFactory::createItemTypeCatalog"
                           << "Unknown HandRequirement" << key;
            }
        }

        itemTypeCatalog.addItemType( std::move( itemType ) );
    }

    qInfo() << "ItemTypeFactory::createItemTypeCatalog";
}

void ItemTypeFactory::saveItemTypeCatalog( const QString& configPath, const ItemTypeCatalog& itemTypeCatalog ) {
    QDir().mkpath( DataFactory::mapPath( configPath ) + "Items/" );

    QStringList keys;
    for ( const QString& key : readTypeKeys( configPath ) ) {
        if ( itemTypeCatalog.itemType( key ) != nullptr ) {
            keys.append( key );
        }
    }

    QStringList newKeys;
    for ( const auto& entry : itemTypeCatalog.itemTypes() ) {
        if ( !keys.contains( entry.first ) ) {
            newKeys.append( entry.first );
        }
    }
    newKeys.sort();
    keys.append( newKeys );

    Json::Value indexJson;
    indexJson[ "ItemTypes" ] = Json::Value( Json::arrayValue );

    for ( const QString& key : keys ) {
        if ( !isValidType( key ) ) {
            qWarning() << "ItemTypeFactory::saveItemTypeCatalog"
                       << "Invalid ItemType key, skipping:" << key;
            continue;
        }

        const ItemTypeModel& itemType = *itemTypeCatalog.itemType( key );

        const QString file = typeFilePath( configPath, key );
        Json::Value itemTypeJson = QFile::exists( file ) ? JsonHelper::loadJsonFile( file ) : Json::Value( Json::objectValue );

        itemTypeJson[ "Name" ] = itemType.name().toStdString();
        itemTypeJson[ "Category" ] = ItemCategoryHelper::toString( itemType.category() );
        itemTypeJson[ "Slot" ] = ItemSlotHelper::toString( itemType.slot() );

        if ( itemType.handRequirement().has_value() ) {
            itemTypeJson[ "HandRequirement" ] = HandRequirementHelper::toString( itemType.handRequirement().value() );
        } else {
            itemTypeJson.removeMember( "HandRequirement" );
        }

        JsonHelper::saveJsonFile( file, itemTypeJson );

        indexJson[ "ItemTypes" ].append( key.toStdString() );
    }

    const QString indexFile = indexFilePath( DataFactory::mapPath( configPath ) );

    qInfo() << "ItemTypeFactory::saveItemTypeCatalog"
            << "[ITEM_TYPES_INDEX_FILE_PATH]" << indexFile;

    JsonHelper::saveJsonFile( indexFile, indexJson );
}

} // namespace Engine
