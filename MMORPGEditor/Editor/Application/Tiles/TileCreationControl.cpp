#include "TileCreationControl.h"

#include <algorithm>

#include <QFileInfo>

#include <MMORPGEditor/Editor/Application/Assets/TextureImportHelper.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/Animation/AnimationModel.h>
#include <MMORPGEngine/Data/DataFactory.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Tile/TileFactory.h>
#include <MMORPGEngine/Data/Tile/TileModel.h>

namespace {
constexpr const char* DATA_PATH = "../../../Data/";

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

void removeUnusedTexture( const QString& mapPath, const QString& textureFile, const Engine::TileCatalog& catalog ) {
    if ( textureFile.isEmpty() ) {
        return;
    }

    for ( const auto& entry : catalog.tiles() ) {
        if ( entry.second.textureFile().compare( textureFile, Qt::CaseInsensitive ) == 0 ) {
            return;
        }
    }

    TextureImportHelper::removeTextureFiles( mapPath, textureFile );
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

bool TileCreationControl::removeTile( int type ) {
    Engine::DataManager& dataManager = Engine::Singleton<Engine::DataManager>::instance();
    const Engine::TileModel* existing = type < 1 ? nullptr : dataManager.tileCatalog().tile( static_cast<uint32_t>( type ) );

    if ( !existing ) {
        setLastError( "Type " + QString::number( type ) + " does not exist." );
        return false;
    }

    const QString textureFile = existing->textureFile();

    dataManager.removeTile( static_cast<uint32_t>( type ) );
    removeUnusedTexture( Engine::DataFactory::mapPath( DATA_PATH ), textureFile, dataManager.tileCatalog() );

    Engine::TileFactory::saveTileCatalog( DATA_PATH, dataManager.tileCatalog() );

    setLastError( "" );
    emit catalogChanged();

    return true;
}

bool TileCreationControl::createTile( int type, const QString& name, const QString& sourceFile, const QString& tagsText, int frameDurationMs, bool isWalkable, bool replace ) {
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

    const QString replacedTextureFile = existing ? existing->textureFile() : QString();

    const QString trimmedName = name.trimmed();

    if ( trimmedName.isEmpty() ) {
        setLastError( "Name the tile." );
        return false;
    }

    const QString sourcePath = TextureImportHelper::toLocalFilePath( sourceFile.trimmed() );

    if ( sourcePath.isEmpty() ) {
        setLastError( "Select a texture." );
        return false;
    }

    const QString textureName = QFileInfo( sourcePath ).completeBaseName();

    if ( !TextureImportHelper::isValidFileName( textureName ) ) {
        setLastError( "Texture file name '" + textureName + "' may only contain lowercase letters, digits, '_' and '-'. Rename the file." );
        return false;
    }

    const QString textureFile = "Textures/Tiles/" + textureName;

    for ( const auto& entry : dataManager.tileCatalog().tiles() ) {
        if ( entry.second.textureFile().compare( textureFile, Qt::CaseInsensitive ) == 0 && entry.first != static_cast<uint32_t>( type ) ) {
            setLastError( "Texture file '" + textureName + "' is already used by type " + QString::number( entry.first ) + " ('" + entry.second.name() + "'). Rename the file." );
            return false;
        }
    }

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

    Engine::TileModel tile;
    tile.setType( static_cast<uint32_t>( type ) );
    tile.setName( trimmedName );
    tile.setTextureFile( textureFile );
    tile.setAnimation( animation );
    tile.setTags( parseTags( tagsText ) );
    tile.setIsWalkable( isWalkable );

    if ( existing ) {
        dataManager.replaceTile( tile );
        removeUnusedTexture( mapPath, replacedTextureFile, dataManager.tileCatalog() );

    } else {
        dataManager.addTile( tile );
    }

    Engine::TileFactory::saveTileCatalog( DATA_PATH, dataManager.tileCatalog() );

    setLastError( "" );
    emit catalogChanged();

    return true;
}
