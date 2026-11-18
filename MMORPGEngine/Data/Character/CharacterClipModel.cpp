#include "CharacterClipModel.h"

namespace Engine {

CharacterClipModel::CharacterClipModel() :
    _frameCount( 0 ),
    _frameDurationMs( 0 ) {
}

CharacterClipModel::CharacterClipModel( int frameCount, int frameDurationMs ) :
    _frameCount( frameCount ),
    _frameDurationMs( frameDurationMs ) {
}

int CharacterClipModel::frameCount() const {
    return _frameCount;
}

int CharacterClipModel::frameDurationMs() const {
    return _frameDurationMs;
}

} // namespace Engine
