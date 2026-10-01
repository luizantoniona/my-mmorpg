#ifndef ENTITYANIMATIONMODEL_H
#define ENTITYANIMATIONMODEL_H

#include <QtTypes>

namespace Engine {

class EntityAnimationModel {
public:
    EntityAnimationModel();
    EntityAnimationModel( int x, int y, int z, double movementSeconds );

    int x() const;
    int y() const;
    int z() const;

    void moveTo( int x, int y, int z, double movementSeconds );

    double offsetX() const;
    double offsetY() const;

private:
    double remainingRatio() const;
    void snap();

private:
    qint64 _startMs;
    int _durationMs;
    int _x;
    int _y;
    int _z;
    int _previousX;
    int _previousY;
};

} // namespace Engine

#endif // ENTITYANIMATIONMODEL_H
