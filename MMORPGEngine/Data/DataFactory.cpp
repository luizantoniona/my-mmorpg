#include "DataFactory.h"

#include <QDebug>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Ground/GroundModel.h>

namespace Engine {

void DataFactory::createGroundCatalog( const QString& groundsFile, GroundCatalog& groundCatalog ) {
    qInfo() << "DataFactory::createGroundCatalog" << "[GROUND_FILE_PATH]" << groundsFile;

    Json::Value json = JsonHelper::loadJsonFile( groundsFile );

    const Json::Value& grounds = json[ "Grounds" ];

    for ( const Json::Value& groundJson : grounds ) {
        GroundModel ground;
        ground.setType( static_cast<uint32_t>( groundJson[ "Type" ].asUInt() ) );
        ground.setName( QString( groundJson[ "Name" ].asCString() ) );
        ground.setFolder( QString( groundJson[ "TextureFolder" ].asCString() ) );

        groundCatalog.addGround( std::move( ground ) );
    }

    qInfo() << "DataFactory::createGroundCatalog";
}

} // namespace Engine
