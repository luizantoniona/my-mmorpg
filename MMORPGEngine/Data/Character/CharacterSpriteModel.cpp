#include "CharacterSpriteModel.h"

namespace Engine {

CharacterSpriteModel::CharacterSpriteModel() :
    _folder(),
    _files() {
}

const QString& CharacterSpriteModel::folder() const {
    return _folder;
}

void CharacterSpriteModel::setFolder( const QString& folder ) {
    _folder = folder;
}

QString CharacterSpriteModel::file( CharacterLayerEnum layer, AppearanceStateEnum state ) const {
    const auto layerIterator = _files.find( layer );

    if ( layerIterator == _files.end() ) {
        return QString();
    }

    const auto stateIterator = layerIterator->second.find( state );

    if ( stateIterator == layerIterator->second.end() ) {
        return QString();
    }

    return stateIterator->second;
}

void CharacterSpriteModel::setFile( CharacterLayerEnum layer, AppearanceStateEnum state, const QString& file ) {
    _files[ layer ][ state ] = file;
}

bool CharacterSpriteModel::hasLayer( CharacterLayerEnum layer ) const {
    return _files.find( layer ) != _files.end();
}

} // namespace Engine
