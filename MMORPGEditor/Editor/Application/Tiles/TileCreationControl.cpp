#include "TileCreationControl.h"

#include <algorithm>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QUrl>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/Animation/AnimationModel.h>
#include <MMORPGEngine/Data/DataFactory.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Tile/TileFactory.h>
#include <MMORPGEngine/Data/Tile/TileModel.h>

namespace {
constexpr const char* DATA_PATH = "../../../Data/";

QString toLocalFilePath( const QString& path ) {
    if ( path.startsWith( "file:" ) ) {
        return QUrl( path ).toLocalFile();
    }

    return path;
}

QStringList parseTags( const QString& tagsText ) {
    QStringList tags;

    for ( const QString& tag : tagsText.split( ",", Qt::SkipEmptyParts ) ) {
        const QString trimmed = tag.trimmed();

        if ( !trimmed.isEmpty() ) {
            tags.append( trimmed );
        }
    }

    return tags;
}

void removeUnusedFolder( const QString& mapPath, const QString& folder, const Engine::TileCatalog& catalog ) {
    if ( folder.isEmpty() ) {
        return;
    }

    for ( const auto& entry : catalog.tiles() ) {
        if ( entry.second.folder() == folder ) {
            return;
        }
    }

    QDir( mapPath + folder ).removeRecursively();
}
} // namespace

TileCreationControl::TileCreationControl( QObject* parent ) :
    QObject( parent ),
    _lastError() {
}

int TileCreationControl::nextType() const {
    const Engine::TileCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().tileCatalog();

    uint32_t maxType = 0;
    for ( const auto& entry : catalog.tiles() ) {
        maxType = std::max( maxType, entry.first );
    }

    return static_cast<int>( maxType ) + 1;
}

QString TileCreationControl::lastError() const {
    return _lastError;
}

int TileCreationControl::nextFreeType( int after ) const {
    const Engine::TileCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().tileCatalog();

    uint32_t type = static_cast<uint32_t>( std::max( after, 0 ) ) + 1;
    while ( catalog.tile( type ) != nullptr ) {
        ++type;
    }

    return static_cast<int>( type );
}

QString TileCreationControl::typeName( int type ) const {
    if ( type < 1 ) {
        return "";
    }

    const Engine::TileCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().tileCatalog();
    const Engine::TileModel* tile = catalog.tile( static_cast<uint32_t>( type ) );

    return tile ? tile->name() : "";
}

void TileCreationControl::setLastError( const QString& error ) {
    _lastError = error;

    emit lastErrorChanged();
}

bool TileCreationControl::createTile( int type, const QString& name, const QString& textureFile, const QString& tagsText, int frameDurationMs, bool isWalkable, bool replace ) {
    if ( type < 1 ) {
        setLastError( "Type must be 1 or higher." );
        return false;
    }

    Engine::DataManager& dataManager = Engine::Singleton<Engine::DataManager>::instance();
    const Engine::TileModel* existing = dataManager.tileCatalog().tile( static_cast<uint32_t>( type ) );

    if ( existing && !replace ) {
        setLastError( "Type " + QString::number( type ) + " is already used by '" + existing->name() + "'." );
        return false;
    }

    const QString replacedFolder = existing ? existing->folder() : QString();

    const QString trimmedName = name.trimmed();

    if ( trimmedName.isEmpty() ) {
        setLastError( "Name the tile." );
        return false;
    }

    const QString sourcePath = toLocalFilePath( textureFile.trimmed() );

    if ( sourcePath.isEmpty() ) {
        setLastError( "Select a texture." );
        return false;
    }

    const QString sourceExtension = "." + QFileInfo( sourcePath ).suffix().toLower();
    const bool isAnimated = Engine::DataFactory::textureExtensions().value( sourceExtension, false );
    const QString extension = isAnimated ? sourceExtension : ".png";

    const QString mapPath = Engine::DataFactory::mapPath( DATA_PATH );
    const QString folder = "Textures/Tiles/" + trimmedName;
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

    Engine::TileModel tile;
    tile.setType( static_cast<uint32_t>( type ) );
    tile.setName( trimmedName );
    tile.setFolder( folder );
    tile.setAnimation( animation );
    tile.setTags( parseTags( tagsText ) );
    tile.setIsWalkable( isWalkable );

    if ( existing ) {
        dataManager.replaceTile( tile );
        removeUnusedFolder( mapPath, replacedFolder, dataManager.tileCatalog() );

    } else {
        dataManager.addTile( tile );
    }

    Engine::TileFactory::saveTileCatalog( DATA_PATH, dataManager.tileCatalog() );

    setLastError( "" );
    emit catalogChanged();

    return true;
}
