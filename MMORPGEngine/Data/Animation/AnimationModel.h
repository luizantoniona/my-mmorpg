#ifndef ANIMATIONMODEL_H
#define ANIMATIONMODEL_H

#include <QImage>
#include <QList>

namespace Engine {

class AnimationModel {
public:
    AnimationModel();
    AnimationModel( const QList<QImage>& frames, int frameDurationMs );

    bool isNull() const;
    bool isAnimated() const;

    QImage firstFrame() const;
    QImage frameAt( qint64 elapsedMs ) const;

    int frameDurationMs() const;

private:
    QList<QImage> _frames;
    int _frameDurationMs;
};

} // namespace Engine

#endif // ANIMATIONMODEL_H
