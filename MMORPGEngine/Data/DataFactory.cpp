#include "DataFactory.h"

#include <QDebug>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Object/ObjectModel.h>
#include <MMORPGEngine/Data/Tile/TileModel.h>

namespace Engine {

void DataFactory::createObjectCatalog( const QString& configPath, ObjectCatalog& objectCatalog ) {
    Json::Value configJson = JsonHelper::loadJsonFile( configPath + "Config.json" );

    const QString mapFolder = QString( configJson[ "ActiveFolder" ].asCString() );

    const QString mapPath = configPath + mapFolder + "/";

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
        const QString texturePath = mapPath + object.folder() + "/" + object.name() + ".png";
        QImage textureImage( texturePath );
        if ( textureImage.isNull() ) {
            qWarning() << "DataFactory::createObjectCatalog" << "Failed to load texture:" << texturePath;

        } else {
            object.setTexture( textureImage );
            qInfo() << "DataFactory::createObjectCatalog" << "Loaded texture:" << texturePath;
        }

        objectCatalog.addObject( std::move( object ) );
    }

    qInfo() << "DataFactory::createObjectCatalog";
}

void DataFactory::createTileCatalog( const QString& configPath, TileCatalog& tileCatalog ) {
    Json::Value configJson = JsonHelper::loadJsonFile( configPath + "Config.json" );

    const QString mapFolder = QString( configJson[ "ActiveFolder" ].asCString() );

    const QString mapPath = configPath + mapFolder + "/";

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    const QString tilesFile = mapPath + QString( mapJson[ "Catalogs" ][ "Tiles" ].asCString() );

    qInfo() << "DataFactory::createTileCatalog" << "[TILES_FILE_PATH]" << tilesFile;

    Json::Value json = JsonHelper::loadJsonFile( tilesFile );

    const Json::Value& tiles = json[ "Tiles" ];

    for ( const Json::Value& tileJson : tiles ) {

        TileModel tile;
        tile.setType( tileJson[ "Type" ].asUInt() );
        tile.setName( QString( tileJson[ "Name" ].asCString() ) );

        // TODO: In the future Tiles will have autotilling. So we must put more textures in each tile catalog and render them accordingly.
        tile.setFolder( QString( tileJson[ "TextureFolder" ].asCString() ) );
        const QString texturePath = mapPath + tile.folder() + "/5.png";
        QImage textureImage( texturePath );
        if ( textureImage.isNull() ) {
            qWarning() << "DataFactory::createTileCatalog" << "Failed to load texture:" << texturePath;

        } else {
            tile.setTexture( textureImage );
            qInfo() << "DataFactory::createTileCatalog" << "Loaded texture:" << texturePath;
        }

        tileCatalog.addTile( std::move( tile ) );
    }

    qInfo() << "DataFactory::createTileCatalog";
}

} // namespace Engine
