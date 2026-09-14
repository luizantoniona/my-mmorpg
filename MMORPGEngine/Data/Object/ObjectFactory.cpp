#include "ObjectFactory.h"

#include <QDebug>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Animation/AnimationModel.h>
#include <MMORPGEngine/Data/DataFactory.h>
#include <MMORPGEngine/Data/Object/ObjectModel.h>
#include <MMORPGEngine/Data/Object/ObjectSizeModel.h>

namespace Engine {

void ObjectFactory::createObjectCatalog( const QString& configPath, ObjectCatalog& objectCatalog ) {
    const QString mapPath = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    const QString objectsFile = mapPath + QString( mapJson[ "Catalogs" ][ "Objects" ].asCString() );

    qInfo() << "ObjectFactory::createObjectCatalog"
            << "[OBJECTS_FILE_PATH]" << objectsFile;

    Json::Value json = JsonHelper::loadJsonFile( objectsFile );

    const Json::Value& objects = json[ "Objects" ];

    for ( const Json::Value& objectJson : objects ) {

        ObjectModel object;
        object.setType( objectJson[ "Type" ].asUInt() );
        object.setName( QString( objectJson[ "Name" ].asCString() ) );

        const Json::Value& footprintJson = objectJson[ "Footprint" ];
        ObjectSizeModel size;
        size.setWidth( footprintJson.get( "Width", 1 ).asInt() );
        size.setHeight( footprintJson.get( "Height", 1 ).asInt() );
        object.setSize( size );

        object.setFolder( QString( objectJson[ "TextureFolder" ].asCString() ) );

        const bool isAnimated = objectJson.get( "IsAnimated", false ).asBool();
        const int frameDurationMs = objectJson.get( "FrameDurationMs", 100 ).asInt();
        const QString extension = isAnimated ? ".gif" : ".png";
        const QString texturePath = mapPath + object.folder() + "/" + object.name() + extension;

        const AnimationModel animation = DataFactory::loadAnimation( texturePath, isAnimated, frameDurationMs );
        if ( animation.isNull() ) {
            qWarning() << "ObjectFactory::createObjectCatalog"
                       << "Failed to load texture:" << texturePath;

        } else {
            object.setAnimation( animation );
            qInfo() << "ObjectFactory::createObjectCatalog"
                    << "Loaded texture:" << texturePath;
        }

        objectCatalog.addObject( std::move( object ) );
    }

    qInfo() << "ObjectFactory::createObjectCatalog";
}

void ObjectFactory::saveObjectCatalog( const QString& configPath, const ObjectCatalog& objectCatalog ) {
    const QString path = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( path + "Map.json" );

    const QString objectsFile = path + QString( mapJson[ "Catalogs" ][ "Objects" ].asCString() );

    Json::Value json;
    json[ "Objects" ] = Json::Value( Json::arrayValue );

    for ( const auto& entry : objectCatalog.objects() ) {
        const ObjectModel& object = entry.second;

        Json::Value objectJson;
        objectJson[ "Type" ] = object.type();
        objectJson[ "Name" ] = object.name().toStdString();

        Json::Value footprintJson;
        footprintJson[ "Width" ] = object.size().width();
        footprintJson[ "Height" ] = object.size().height();
        objectJson[ "Footprint" ] = footprintJson;

        objectJson[ "TextureFolder" ] = object.folder().toStdString();

        if ( object.animation().isAnimated() ) {
            objectJson[ "IsAnimated" ] = true;
            objectJson[ "FrameDurationMs" ] = object.animation().frameDurationMs();
        }

        json[ "Objects" ].append( objectJson );
    }

    qInfo() << "ObjectFactory::saveObjectCatalog"
            << "[OBJECTS_FILE_PATH]" << objectsFile;

    JsonHelper::saveJsonFile( objectsFile, json );
}

} // namespace Engine
