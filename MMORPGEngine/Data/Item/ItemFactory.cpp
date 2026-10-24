#include "ItemFactory.h"

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include <QDebug>
#include <QFile>
#include <QRegularExpression>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Attack/AttackFactory.h>
#include <MMORPGEngine/Data/DataFactory.h>
#include <MMORPGEngine/Data/Item/ItemModel.h>
#include <MMORPGEngine/Data/Item/ItemTypeFactory.h>

namespace {

bool isValidItemId( const Json::Value& idJson ) {
    static const QRegularExpression pattern( "^[A-Z0-9_]+$" );

    return idJson.isString() && pattern.match( QString::fromStdString( idJson.asString() ) ).hasMatch();
}

} // namespace

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

            if ( !itemJson.isObject() || !itemJson.isMember( "Id" ) || !itemJson.isMember( "Name" ) ) {
                qWarning() << "ItemFactory::createItemCatalog"
                           << "Invalid Item, skipping";
                continue;
            }

            if ( !JsonHelper::hasValidFields( itemJson, { { "Name", JsonFieldTypeEnum::STRING }, { "TextureFile", JsonFieldTypeEnum::STRING }, { "IsAnimated", JsonFieldTypeEnum::BOOL }, { "FrameDurationMs", JsonFieldTypeEnum::INT }, { "Damage", JsonFieldTypeEnum::NUMBER }, { "Attack", JsonFieldTypeEnum::OBJECT } } ) || !isValidItemId( itemJson[ "Id" ] ) ) {
                qWarning() << "ItemFactory::createItemCatalog"
                           << "Item Id must be a key text (A-Z, 0-9, _) and Name a text, skipping";
                continue;
            }

            const QString id = QString::fromStdString( itemJson[ "Id" ].asString() );
            if ( itemCatalog.item( id ) != nullptr ) {
                qWarning() << "ItemFactory::createItemCatalog"
                           << "Duplicated Item Id, skipping:" << id;
                continue;
            }

            AttackModel attack;
            const bool hasAttack = itemJson.isMember( "Attack" );

            if ( hasAttack && !AttackFactory::fromJson( itemJson[ "Attack" ], attack ) ) {
                qWarning() << "ItemFactory::createItemCatalog"
                           << "Invalid Attack, skipping Item:" << id;
                continue;
            }

            ItemModel item;
            item.setId( id );

            if ( hasAttack ) {
                item.setAttack( attack );
            }

            item.setIdItemType( idItemType );
            item.setName( QString( itemJson[ "Name" ].asCString() ) );

            if ( itemJson.isMember( "Damage" ) ) {
                item.setDamage( itemJson[ "Damage" ].asDouble() );
            }

            if ( itemJson.isMember( "TextureFile" ) ) {
                item.setTextureFile( QString::fromStdString( itemJson[ "TextureFile" ].asString() ) );

                const bool isAnimated = itemJson.get( "IsAnimated", false ).asBool();
                const int frameDurationMs = itemJson.get( "FrameDurationMs", 100 ).asInt();
                const QString texturePath = DataFactory::resolveTexturePath( mapPath + item.textureFile(), isAnimated );

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
            std::map<std::string, Json::Value> attacksById;
            for ( const Json::Value& previousJson : json[ "Items" ] ) {
                if ( previousJson.isObject() && previousJson[ "Id" ].isString() && previousJson.isMember( "Attack" ) ) {
                    attacksById[ previousJson[ "Id" ].asString() ] = previousJson[ "Attack" ];
                }
            }

            Json::Value itemsJson( Json::arrayValue );

            for ( const ItemModel* item : items ) {
                Json::Value itemJson;
                itemJson[ "Id" ] = item->id().toStdString();
                itemJson[ "Name" ] = item->name().toStdString();

                const auto attackIt = attacksById.find( item->id().toStdString() );
                if ( item->attack().has_value() && attackIt != attacksById.end() ) {
                    itemJson[ "Attack" ] = attackIt->second;
                }

                if ( item->damage() > 0.0 ) {
                    itemJson[ "Damage" ] = item->damage();
                }

                if ( !item->textureFile().isEmpty() ) {
                    itemJson[ "TextureFile" ] = item->textureFile().toStdString();

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
