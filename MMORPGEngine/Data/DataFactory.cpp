#include "DataFactory.h"

#include <QDebug>
#include <QImageReader>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Animation/AnimationModel.h>
#include <MMORPGEngine/Data/Object/ObjectModel.h>
#include <MMORPGEngine/Data/Tile/TileModel.h>

namespace Engine {

void DataFactory::createObjectCatalog( const QString& configPath, ObjectCatalog& objectCatalog ) {
    const QString mapPath = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    const QString objectsFile = mapPath + QString( mapJson[ "Catalogs" ][ "Objects" ].asCString() );

    qInfo() << "DataFactory::createObjectCatalog" << "[OBJECTS_FILE_PATH]" << objectsFile;

    Json::Value json = JsonHelper::loadJsonFile( objectsFile );

    const Json::Value& objects = json[ "Objects" ];

    for ( const Json::Value& objectJson : objects ) {

        ObjectModel object;
        object.setType( objectJson[ "Type" ].asUInt() );
        object.setName( QString( objectJson[ "Name" ].asCString() ) );

        // TODO: Setar coisas especificas do Object
        // ObjectSizeModel

        object.setFolder( QString( objectJson[ "TextureFolder" ].asCString() ) );

        const bool isAnimated = objectJson.get( "IsAnimated", false ).asBool();
        const int frameDurationMs = objectJson.get( "FrameDurationMs", 100 ).asInt();
        const QString extension = isAnimated ? ".gif" : ".png";
        const QString texturePath = mapPath + object.folder() + "/" + object.name() + extension;

        const AnimationModel animation = loadAnimation( texturePath, isAnimated, frameDurationMs );
        if ( animation.isNull() ) {
            qWarning() << "DataFactory::createObjectCatalog" << "Failed to load texture:" << texturePath;

        } else {
            object.setAnimation( animation );
            qInfo() << "DataFactory::createObjectCatalog" << "Loaded texture:" << texturePath;
        }

        objectCatalog.addObject( std::move( object ) );
    }

    qInfo() << "DataFactory::createObjectCatalog";
}

void DataFactory::createTileCatalog( const QString& configPath, TileCatalog& tileCatalog ) {
    const QString mapPath = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    const QString tilesFile = mapPath + QString( mapJson[ "Catalogs" ][ "Tiles" ].asCString() );

    qInfo() << "DataFactory::createTileCatalog" << "[TILES_FILE_PATH]" << tilesFile;

    Json::Value json = JsonHelper::loadJsonFile( tilesFile );

    const Json::Value& tiles = json[ "Tiles" ];

    for ( const Json::Value& tileJson : tiles ) {

        TileModel tile;
        tile.setType( tileJson[ "Type" ].asUInt() );
        tile.setName( QString( tileJson[ "Name" ].asCString() ) );
        tile.setFolder( QString( tileJson[ "TextureFolder" ].asCString() ) );

        const bool isAnimated = tileJson.get( "IsAnimated", false ).asBool();
        const int frameDurationMs = tileJson.get( "FrameDurationMs", 100 ).asInt();
        const QString extension = isAnimated ? ".gif" : ".png";
        const QString texturePath = mapPath + tile.folder() + "/" + tile.name() + extension;

        const AnimationModel animation = loadAnimation( texturePath, isAnimated, frameDurationMs );
        if ( animation.isNull() ) {
            qWarning() << "DataFactory::createTileCatalog" << "Failed to load texture:" << texturePath;

        } else {
            tile.setAnimation( animation );
            qInfo() << "DataFactory::createTileCatalog" << "Loaded texture:" << texturePath;
        }

        tileCatalog.addTile( std::move( tile ) );
    }

    qInfo() << "DataFactory::createTileCatalog";
}

QString DataFactory::mapPath( const QString& configPath ) {
    Json::Value configJson = JsonHelper::loadJsonFile( configPath + "Config.json" );

    const QString mapFolder = QString( configJson[ "ActiveFolder" ].asCString() );

    return configPath + mapFolder + "/";
}

AnimationModel DataFactory::loadAnimation( const QString& texturePath, bool isAnimated, int frameDurationMs ) {
    if ( !isAnimated ) {
        return AnimationModel( { QImage( texturePath ) }, frameDurationMs );
    }

    QImageReader reader( texturePath );

    QList<QImage> frames;
    while ( reader.canRead() ) {
        frames.append( reader.read() );
    }

    return AnimationModel( frames, frameDurationMs );
}

} // namespace Engine
