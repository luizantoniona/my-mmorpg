#include "AnimationModel.h"

namespace Engine {

AnimationModel::AnimationModel() :
    _frames(),
    _frameDurationMs( 100 ) {
}

AnimationModel::AnimationModel( const QList<QImage>& frames, int frameDurationMs ) :
    _frames( frames ),
    _frameDurationMs( frameDurationMs > 0 ? frameDurationMs : 100 ) {
}

bool AnimationModel::isNull() const {
    return _frames.isEmpty() || _frames.first().isNull();
}

bool AnimationModel::isAnimated() const {
    return _frames.size() > 1;
}

QImage AnimationModel::firstFrame() const {
    return _frames.isEmpty() ? QImage() : _frames.first();
}

QImage AnimationModel::frameAt( qint64 elapsedMs ) const {
    if ( _frames.isEmpty() ) {
        return QImage();
    }

    if ( _frames.size() == 1 ) {
        return _frames.first();
    }

    const int index = static_cast<int>( ( elapsedMs / _frameDurationMs ) % _frames.size() );

    return _frames.at( index );
}

int AnimationModel::frameDurationMs() const {
    return _frameDurationMs;
}

} // namespace Engine
