#include "ItemFactory.h"

#include <algorithm>
#include <vector>

#include <QDebug>
#include <QFile>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/DataFactory.h>
#include <MMORPGEngine/Data/Item/ItemModel.h>
#include <MMORPGEngine/Data/Item/ItemTypeFactory.h>

namespace Engine {

void ItemFactory::createItemCatalog( const QString& configPath, const ItemTypeCatalog& itemTypeCatalog, ItemCatalog& itemCatalog ) {
    const QString mapPath = DataFactory::mapPath( configPath );

    for ( const QString& idItemType : ItemTypeFactory::readTypeKeys( configPath ) ) {

        if ( itemTypeCatalog.itemType( idItemType ) == nullptr ) {
            qWarning() << "ItemFactory::createItemCatalog"
                       << "Unknown ItemType, skipping Items:" << idItemType;
            continue;
        }

        const Json::Value json = JsonHelper::loadJsonFile( ItemTypeFactory::typeFilePath( configPath, idItemType ) );

        for ( const Json::Value& itemJson : json[ "Items" ] ) {

            if ( !itemJson.isMember( "Id" ) || !itemJson.isMember( "Name" ) ) {
                qWarning() << "ItemFactory::createItemCatalog"
                           << "Invalid Item, skipping";
                continue;
            }

            const uint32_t id = itemJson[ "Id" ].asUInt();
            if ( itemCatalog.item( id ) != nullptr ) {
                qWarning() << "ItemFactory::createItemCatalog"
                           << "Duplicated Item Id, skipping:" << id;
                continue;
            }

            ItemModel item;
            item.setId( id );
            item.setIdItemType( idItemType );
            item.setName( QString( itemJson[ "Name" ].asCString() ) );

            if ( itemJson.isMember( "TextureFolder" ) ) {
                item.setFolder( QString( itemJson[ "TextureFolder" ].asCString() ) );

                const bool isAnimated = itemJson.get( "IsAnimated", false ).asBool();
                const int frameDurationMs = itemJson.get( "FrameDurationMs", 100 ).asInt();
                const QString texturePath = DataFactory::resolveTexturePath( mapPath + item.folder() + "/" + item.name(), isAnimated );

                const AnimationModel animation = DataFactory::loadAnimation( texturePath, isAnimated, frameDurationMs );
                if ( animation.isNull() ) {
                    qWarning() << "ItemFactory::createItemCatalog"
                               << "Failed to load texture:" << texturePath;

                } else {
                    item.setAnimation( animation );
                }
            }

            itemCatalog.addItem( std::move( item ) );
        }
    }

    qInfo() << "ItemFactory::createItemCatalog";
}

void ItemFactory::saveItemCatalog( const QString& configPath, const ItemCatalog& itemCatalog ) {
    const QStringList keys = ItemTypeFactory::readTypeKeys( configPath );

    for ( const auto& entry : itemCatalog.items() ) {
        if ( !keys.contains( entry.second.idItemType() ) ) {
            qWarning() << "ItemFactory::saveItemCatalog"
                       << "Unknown ItemType, skipping Item:" << entry.first;
        }
    }

    for ( const QString& idItemType : keys ) {
        const QString file = ItemTypeFactory::typeFilePath( configPath, idItemType );

        if ( !QFile::exists( file ) ) {
            qWarning() << "ItemFactory::saveItemCatalog"
                       << "ItemType file not found, skipping:" << file;
            continue;
        }

        std::vector<const ItemModel*> items;
        for ( const auto& entry : itemCatalog.items() ) {
            if ( entry.second.idItemType() == idItemType ) {
                items.push_back( &entry.second );
            }
        }

        std::sort( items.begin(), items.end(), []( const ItemModel* left, const ItemModel* right ) { return left->id() < right->id(); } );

        Json::Value json = JsonHelper::loadJsonFile( file );

        if ( items.empty() ) {
            json.removeMember( "Items" );

        } else {
            Json::Value itemsJson( Json::arrayValue );

            for ( const ItemModel* item : items ) {
                Json::Value itemJson;
                itemJson[ "Id" ] = item->id();
                itemJson[ "Name" ] = item->name().toStdString();

                if ( !item->folder().isEmpty() ) {
                    itemJson[ "TextureFolder" ] = item->folder().toStdString();

                    if ( item->animation().isAnimated() ) {
                        itemJson[ "IsAnimated" ] = true;
                        itemJson[ "FrameDurationMs" ] = item->animation().frameDurationMs();
                    }
                }

                itemsJson.append( itemJson );
            }

            json[ "Items" ] = itemsJson;
        }

        JsonHelper::saveJsonFile( file, json );
    }

    qInfo() << "ItemFactory::saveItemCatalog";
}

} // namespace Engine
