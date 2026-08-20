#include "DataFactory.h"

#include <QDebug>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Ground/GroundModel.h>

namespace Engine {

void DataFactory::createGroundCatalog( const QString& configPath, GroundCatalog& groundCatalog ) {
    Json::Value configJson = JsonHelper::loadJsonFile( configPath + "Config.json" );

    const QString mapFolder = QString( configJson[ "ActiveFolder" ].asCString() );

    const QString mapPath = configPath + mapFolder + "/";

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    const QString groundFile = mapPath + QString( mapJson[ "Catalogs" ][ "Grounds" ].asCString() );

    qInfo() << "DataFactory::createGroundCatalog" << "[GROUND_FILE_PATH]" << groundFile;

    Json::Value json = JsonHelper::loadJsonFile( groundFile );

    const Json::Value& grounds = json[ "Grounds" ];

    for ( const Json::Value& groundJson : grounds ) {
        GroundModel ground;
        ground.setType( static_cast<uint32_t>( groundJson[ "Type" ].asUInt() ) );
        ground.setName( QString( groundJson[ "Name" ].asCString() ) );
        ground.setFolder( QString( groundJson[ "TextureFolder" ].asCString() ) );

        const QString texturePath = mapPath + ground.folder() + "/" + ground.name() + ".png";
        QImage texture( texturePath );
        if ( texture.isNull() ) {
            qWarning() << "DataFactory::createGroundCatalog" << "Failed to load texture:" << texturePath;

        } else {
            ground.setTexture( texture );
            qInfo() << "DataFactory::createGroundCatalog" << "Loaded texture:" << texturePath;
        }

        groundCatalog.addGround( std::move( ground ) );
    }

    qInfo() << "DataFactory::createGroundCatalog";
}

} // namespace Engine
