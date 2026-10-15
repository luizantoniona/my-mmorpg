#ifndef ENTITYANIMATIONMODEL_H
#define ENTITYANIMATIONMODEL_H

#include <QtTypes>

#include <MMORPGEngine/Data/Appearance/AppearanceDirectionEnum.h>

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

    AppearanceDirectionEnum facing() const;

    bool isMoving() const;

    void startAttack( int targetX, int targetY, double castSeconds );
    bool isAttacking() const;
    qint64 attackElapsedMs() const;

private:
    double remainingRatio() const;
    void snap();

private:
    qint64 _startMs;
    qint64 _attackStartMs;
    int _durationMs;
    int _attackDurationMs;
    int _x;
    int _y;
    int _z;
    int _previousX;
    int _previousY;
    AppearanceDirectionEnum _facing;
};

} // namespace Engine

#endif // ENTITYANIMATIONMODEL_H
