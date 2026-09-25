#include "ObjectCreationControl.h"

#include <algorithm>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QUrl>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/Animation/AnimationModel.h>
#include <MMORPGEngine/Data/DataFactory.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Object/ObjectFactory.h>
#include <MMORPGEngine/Data/Object/ObjectModel.h>
#include <MMORPGEngine/Data/Object/ObjectSizeModel.h>

namespace {
constexpr const char* DATA_PATH = "../../../Data/";

QString toLocalFilePath( const QString& path ) {
    if ( path.startsWith( "file:" ) ) {
        return QUrl( path ).toLocalFile();
    }

    return path;
}
} // namespace

ObjectCreationControl::ObjectCreationControl( QObject* parent ) :
    QObject( parent ),
    _lastError() {
}

int ObjectCreationControl::nextType() const {
    const Engine::ObjectCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().objectCatalog();

    uint32_t maxType = 0;
    for ( const auto& entry : catalog.objects() ) {
        maxType = std::max( maxType, entry.first );
    }

    return static_cast<int>( maxType ) + 1;
}

QString ObjectCreationControl::lastError() const {
    return _lastError;
}

void ObjectCreationControl::setLastError( const QString& error ) {
    _lastError = error;

    emit lastErrorChanged();
}

bool ObjectCreationControl::createObject( const QString& name, const QString& textureFile, int width, int height, int frameDurationMs ) {
    const QString trimmedName = name.trimmed();

    if ( trimmedName.isEmpty() ) {
        setLastError( "Name the object." );
        return false;
    }

    const QString sourcePath = toLocalFilePath( textureFile.trimmed() );

    if ( sourcePath.isEmpty() ) {
        setLastError( "Select a texture." );
        return false;
    }

    if ( width < 1 || height < 1 ) {
        setLastError( "Footprint must be at least 1x1." );
        return false;
    }

    const QString sourceExtension = "." + QFileInfo( sourcePath ).suffix().toLower();
    const bool isAnimated = Engine::DataFactory::textureExtensions().value( sourceExtension, false );
    const QString extension = isAnimated ? sourceExtension : ".png";

    const QString mapPath = Engine::DataFactory::mapPath( DATA_PATH );
    const QString folder = "Textures/Objects/" + trimmedName;
    const QString destDir = mapPath + folder;

    QDir().mkpath( destDir );

    const QString destPath = destDir + "/" + trimmedName + extension;

    if ( QFile::exists( destPath ) ) {
        QFile::remove( destPath );
    }

    if ( !QFile::copy( sourcePath, destPath ) ) {
        setLastError( "Failed to copy texture." );
        return false;
    }

    const Engine::AnimationModel animation = Engine::DataFactory::loadAnimation( destPath, isAnimated, frameDurationMs > 0 ? frameDurationMs : 100 );
    if ( animation.isNull() ) {
        setLastError( "Failed to load the copied texture." );
        return false;
    }

    Engine::ObjectSizeModel size;
    size.setWidth( width );
    size.setHeight( height );

    Engine::ObjectModel object;
    object.setType( static_cast<uint32_t>( nextType() ) );
    object.setName( trimmedName );
    object.setFolder( folder );
    object.setAnimation( animation );
    object.setSize( size );

    Engine::Singleton<Engine::DataManager>::instance().addObject( object );
    Engine::ObjectFactory::saveObjectCatalog( DATA_PATH, Engine::Singleton<Engine::DataManager>::instance().objectCatalog() );

    setLastError( "" );
    emit catalogChanged();

    return true;
}
