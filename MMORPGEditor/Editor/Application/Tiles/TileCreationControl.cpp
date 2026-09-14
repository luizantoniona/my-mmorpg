#include "TileCreationControl.h"

#include <algorithm>

#include <QDir>
#include <QFile>
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

void TileCreationControl::setLastError( const QString& error ) {
    _lastError = error;

    emit lastErrorChanged();
}

bool TileCreationControl::createTile( const QString& name, const QString& textureFile, const QString& tagsText, int frameDurationMs ) {
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

    const bool isAnimated = sourcePath.endsWith( ".gif", Qt::CaseInsensitive );
    const QString extension = isAnimated ? ".gif" : ".png";

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
    tile.setType( static_cast<uint32_t>( nextType() ) );
    tile.setName( trimmedName );
    tile.setFolder( folder );
    tile.setAnimation( animation );
    tile.setTags( parseTags( tagsText ) );

    Engine::Singleton<Engine::DataManager>::instance().addTile( tile );
    Engine::TileFactory::saveTileCatalog( DATA_PATH, Engine::Singleton<Engine::DataManager>::instance().tileCatalog() );

    setLastError( "" );
    emit catalogChanged();

    return true;
}
