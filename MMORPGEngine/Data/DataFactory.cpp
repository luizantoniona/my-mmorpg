#include "DataFactory.h"

#include <QDebug>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Tile/TileTextureModel.h>

namespace Engine {

void DataFactory::createTileTextureCatalog( const QString& configPath, TileTextureCatalog& tileTextureCatalog ) {
    Json::Value configJson = JsonHelper::loadJsonFile( configPath + "Config.json" );

    const QString mapFolder = QString( configJson[ "ActiveFolder" ].asCString() );

    const QString mapPath = configPath + mapFolder + "/";

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    const QString tilesFile = mapPath + QString( mapJson[ "Catalogs" ][ "Tiles" ].asCString() );

    qInfo() << "DataFactory::createTileTextureCatalog" << "[TILES_FILE_PATH]" << tilesFile;

    Json::Value json = JsonHelper::loadJsonFile( tilesFile );

    const Json::Value& tiles = json[ "Tiles" ];

    for ( const Json::Value& tileJson : tiles ) {

        TileTextureModel texture;
        texture.setType( tileJson[ "Type" ].asUInt() );
        texture.setName( QString( tileJson[ "Name" ].asCString() ) );
        texture.setFolder( QString( tileJson[ "TextureFolder" ].asCString() ) );

        const QString texturePath = mapPath + texture.folder() + "/" + texture.name() + ".png";

        QImage textureImage( texturePath );
        if ( textureImage.isNull() ) {
            qWarning() << "DataFactory::createTileTextureCatalog" << "Failed to load texture:" << texturePath;

        } else {
            texture.setTexture( textureImage );
            qInfo() << "DataFactory::createTileTextureCatalog" << "Loaded texture:" << texturePath;
        }

        tileTextureCatalog.addTexture( std::move( texture ) );
    }

    qInfo() << "DataFactory::createTileTextureCatalog";
}

void DataFactory::createObjectTextureCatalog( const QString& configPath, ObjectTextureCatalog& objectTextureCatalog ) {
    // TODO: Load object textures
}

} // namespace Engine
