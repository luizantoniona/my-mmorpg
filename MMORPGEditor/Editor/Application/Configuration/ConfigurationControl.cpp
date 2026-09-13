#include "ConfigurationControl.h"

#include <QDir>
#include <QFile>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/World/WorldFactory.h>

namespace {
constexpr const char* DATA_PATH = "../../../Data/";
} // namespace

ConfigurationControl::ConfigurationControl( QObject* parent ) :
    QObject( parent ),
    _mapFolders(),
    _activeFolder(),
    _mapName(),
    _mapWidth( 0 ),
    _mapHeight( 0 ),
    _floorCount( 0 ),
    _lastError() {

    reloadMapFolders();
    reloadActiveMap();
}

QStringList ConfigurationControl::mapFolders() const {
    return _mapFolders;
}

QString ConfigurationControl::activeFolder() const {
    return _activeFolder;
}

QString ConfigurationControl::mapName() const {
    return _mapName;
}

int ConfigurationControl::mapWidth() const {
    return _mapWidth;
}

int ConfigurationControl::mapHeight() const {
    return _mapHeight;
}

int ConfigurationControl::floorCount() const {
    return _floorCount;
}

QString ConfigurationControl::lastError() const {
    return _lastError;
}

void ConfigurationControl::setLastError( const QString& error ) {
    _lastError = error;

    emit lastErrorChanged();
}

void ConfigurationControl::reloadMapFolders() {
    QDir dataDir( DATA_PATH );

    QStringList folders;
    for ( const QString& entry : dataDir.entryList( QDir::Dirs | QDir::NoDotAndDotDot ) ) {
        if ( QFile::exists( dataDir.filePath( entry ) + "/Map.json" ) ) {
            folders.append( entry );
        }
    }

    _mapFolders = folders;

    emit mapFoldersChanged();
}

void ConfigurationControl::reloadActiveMap() {
    const Json::Value configJson = Engine::JsonHelper::loadJsonFile( QString( DATA_PATH ) + "Config.json" );
    _activeFolder = QString::fromStdString( configJson[ "ActiveFolder" ].asString() );

    const Json::Value mapJson = Engine::JsonHelper::loadJsonFile( QString( DATA_PATH ) + _activeFolder + "/Map.json" );
    _mapName = QString::fromStdString( mapJson[ "Name" ].asString() );
    _mapWidth = mapJson[ "Width" ].asInt();
    _mapHeight = mapJson[ "Height" ].asInt();
    _floorCount = static_cast<int>( mapJson[ "Floors" ].size() );

    emit activeFolderChanged();
}

bool ConfigurationControl::selectFolder( const QString& folder ) {
    if ( !_mapFolders.contains( folder ) ) {
        setLastError( "Unknown map folder." );
        return false;
    }

    Json::Value configJson;
    configJson[ "ActiveFolder" ] = folder.toStdString();

    if ( !Engine::JsonHelper::saveJsonFile( QString( DATA_PATH ) + "Config.json", configJson ) ) {
        setLastError( "Failed to write Config.json." );
        return false;
    }

    Engine::Singleton<Engine::DataManager>::instance().reload( DATA_PATH );

    reloadActiveMap();

    setLastError( "" );

    return true;
}

bool ConfigurationControl::resizeMap( int width, int height ) {
    if ( width < _mapWidth || height < _mapHeight ) {
        setLastError( "New size must be greater than or equal to the current size." );
        return false;
    }

    if ( !Engine::WorldFactory::resizeWorld( DATA_PATH, static_cast<uint32_t>( width ), static_cast<uint32_t>( height ) ) ) {
        setLastError( "Failed to resize the map." );
        return false;
    }

    reloadActiveMap();

    setLastError( "" );

    return true;
}
