#ifndef ENTITYPOSITIONMODEL_H
#define ENTITYPOSITIONMODEL_H

namespace Engine {

class EntityPositionModel {
public:
    EntityPositionModel();
    ~EntityPositionModel();

    int x() const;
    void setX( int x );

    int y() const;
    void setY( int y );

    int z() const;
    void setZ( int z );

    int respawnX() const;
    void setRespawnX( int respawnX );

    int respawnY() const;
    void setRespawnY( int respawnY );

    int respawnZ() const;
    void setRespawnZ( int respawnZ );

private:
    int _x;
    int _y;
    int _z;
    int _respawnX;
    int _respawnY;
    int _respawnZ;
};

} // namespace Engine

#endif // ENTITYPOSITIONMODEL_H
