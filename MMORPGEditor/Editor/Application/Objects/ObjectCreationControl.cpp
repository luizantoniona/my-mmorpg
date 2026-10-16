#include "ObjectCreationControl.h"

#include <algorithm>

#include <MMORPGEditor/Editor/Application/Assets/TextureImportHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/Animation/AnimationModel.h>
#include <MMORPGEngine/Data/DataFactory.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Object/ObjectFactory.h>
#include <MMORPGEngine/Data/Object/ObjectModel.h>
#include <MMORPGEngine/Data/Object/ObjectSizeModel.h>

namespace {
constexpr const char* DATA_PATH = "../../../Data/";

void removeUnusedTexture( const QString& mapPath, const QString& textureFile, const Engine::ObjectCatalog& catalog ) {
    if ( textureFile.isEmpty() ) {
        return;
    }

    for ( const auto& entry : catalog.objects() ) {
        if ( entry.second.textureFile().compare( textureFile, Qt::CaseInsensitive ) == 0 ) {
            return;
        }
    }

    TextureImportHelper::removeTextureFiles( mapPath, textureFile );
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

int ObjectCreationControl::nextFreeType( int after ) const {
    const Engine::ObjectCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().objectCatalog();

    uint32_t type = static_cast<uint32_t>( std::max( after, 0 ) ) + 1;
    while ( catalog.object( type ) != nullptr ) {
        ++type;
    }

    return static_cast<int>( type );
}

QString ObjectCreationControl::typeName( int type ) const {
    if ( type < 1 ) {
        return "";
    }

    const Engine::ObjectCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().objectCatalog();
    const Engine::ObjectModel* object = catalog.object( static_cast<uint32_t>( type ) );

    return object ? object->name() : "";
}

void ObjectCreationControl::setLastError( const QString& error ) {
    _lastError = error;

    emit lastErrorChanged();
}

bool ObjectCreationControl::createObject( int type, const QString& name, const QString& sourceFile, int width, int height, int frameDurationMs, bool isWalkable, bool replace ) {
    if ( type < 1 ) {
        setLastError( "Type must be 1 or higher." );
        return false;
    }

    Engine::DataManager& dataManager = Engine::Singleton<Engine::DataManager>::instance();
    const Engine::ObjectModel* existing = dataManager.objectCatalog().object( static_cast<uint32_t>( type ) );

    if ( existing && !replace ) {
        setLastError( "Type " + QString::number( type ) + " is already used by '" + existing->name() + "'." );
        return false;
    }

    const QString replacedTextureFile = existing ? existing->textureFile() : QString();

    const QString trimmedName = name.trimmed();

    if ( trimmedName.isEmpty() ) {
        setLastError( "Name the object." );
        return false;
    }

    const QString sourcePath = TextureImportHelper::toLocalFilePath( sourceFile.trimmed() );

    if ( sourcePath.isEmpty() ) {
        setLastError( "Select a texture." );
        return false;
    }

    if ( width < 1 || height < 1 ) {
        setLastError( "Footprint must be at least 1x1." );
        return false;
    }

    const QString textureFile = "Textures/Objects/" + QString::number( type );

    const bool isAnimated = TextureImportHelper::isAnimatedSource( sourcePath );
    const Engine::AnimationModel animation = Engine::DataFactory::loadAnimation( sourcePath, isAnimated, frameDurationMs > 0 ? frameDurationMs : 100 );

    if ( animation.isNull() ) {
        setLastError( "Failed to load the texture." );
        return false;
    }

    const QString mapPath = Engine::DataFactory::mapPath( DATA_PATH );

    if ( !TextureImportHelper::importTexture( sourcePath, mapPath, textureFile, TextureImportHelper::destinationExtension( sourcePath ) ) ) {
        setLastError( "Failed to copy texture." );
        return false;
    }

    Engine::ObjectSizeModel size;
    size.setWidth( width );
    size.setHeight( height );

    Engine::ObjectModel object;
    object.setType( static_cast<uint32_t>( type ) );
    object.setName( trimmedName );
    object.setTextureFile( textureFile );
    object.setAnimation( animation );
    object.setSize( size );
    object.setIsWalkable( isWalkable );

    if ( existing ) {
        dataManager.replaceObject( object );
        removeUnusedTexture( mapPath, replacedTextureFile, dataManager.objectCatalog() );

    } else {
        dataManager.addObject( object );
    }

    Engine::ObjectFactory::saveObjectCatalog( DATA_PATH, dataManager.objectCatalog() );

    setLastError( "" );
    emit catalogChanged();

    return true;
}
