#include "TileFactory.h"

#include <QDebug>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Animation/AnimationModel.h>
#include <MMORPGEngine/Data/DataFactory.h>
#include <MMORPGEngine/Data/Tile/TileModel.h>

namespace Engine {

void TileFactory::createTileCatalog( const QString& configPath, TileCatalog& tileCatalog ) {
    const QString mapPath = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    const QString tilesFile = mapPath + QString( mapJson[ "Catalogs" ][ "Tiles" ].asCString() );

    qInfo() << "TileFactory::createTileCatalog"
            << "[TILES_FILE_PATH]" << tilesFile;

    Json::Value json = JsonHelper::loadJsonFile( tilesFile );

    const Json::Value& tiles = json[ "Tiles" ];

    for ( const Json::Value& tileJson : tiles ) {

        TileModel tile;
        tile.setType( tileJson[ "Type" ].asUInt() );
        tile.setName( QString( tileJson[ "Name" ].asCString() ) );
        tile.setFolder( QString( tileJson[ "TextureFolder" ].asCString() ) );

        QStringList tags;
        const Json::Value& tagsJson = tileJson[ "Tags" ];
        if ( tagsJson.isArray() ) {
            for ( const Json::Value& tagJson : tagsJson ) {
                tags.append( QString( tagJson.asCString() ) );
            }
        }
        tile.setTags( tags );
        tile.setIsWalkable( tileJson.get( "IsWalkable", true ).asBool() );

        const bool isAnimated = tileJson.get( "IsAnimated", false ).asBool();
        const int frameDurationMs = tileJson.get( "FrameDurationMs", 100 ).asInt();
        const QString extension = isAnimated ? ".gif" : ".png";
        const QString texturePath = mapPath + tile.folder() + "/" + tile.name() + extension;

        const AnimationModel animation = DataFactory::loadAnimation( texturePath, isAnimated, frameDurationMs );
        if ( animation.isNull() ) {
            qWarning() << "TileFactory::createTileCatalog"
                       << "Failed to load texture:" << texturePath;

        } else {
            tile.setAnimation( animation );
            qInfo() << "TileFactory::createTileCatalog"
                    << "Loaded texture:" << texturePath;
        }

        tileCatalog.addTile( std::move( tile ) );
    }

    qInfo() << "TileFactory::createTileCatalog";
}

void TileFactory::saveTileCatalog( const QString& configPath, const TileCatalog& tileCatalog ) {
    const QString path = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( path + "Map.json" );

    const QString tilesFile = path + QString( mapJson[ "Catalogs" ][ "Tiles" ].asCString() );

    Json::Value json;
    json[ "Tiles" ] = Json::Value( Json::arrayValue );

    for ( const auto& entry : tileCatalog.tiles() ) {
        const TileModel& tile = entry.second;

        Json::Value tileJson;
        tileJson[ "Type" ] = tile.type();
        tileJson[ "Name" ] = tile.name().toStdString();
        tileJson[ "TextureFolder" ] = tile.folder().toStdString();
        tileJson[ "IsWalkable" ] = tile.isWalkable();

        if ( tile.animation().isAnimated() ) {
            tileJson[ "IsAnimated" ] = true;
            tileJson[ "FrameDurationMs" ] = tile.animation().frameDurationMs();
        }

        Json::Value tagsJson( Json::arrayValue );
        for ( const QString& tag : tile.tags() ) {
            tagsJson.append( tag.toStdString() );
        }
        tileJson[ "Tags" ] = tagsJson;

        json[ "Tiles" ].append( tileJson );
    }

    qInfo() << "TileFactory::saveTileCatalog"
            << "[TILES_FILE_PATH]" << tilesFile;

    JsonHelper::saveJsonFile( tilesFile, json );
}

} // namespace Engine
