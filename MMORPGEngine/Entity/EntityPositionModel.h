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

private:
    int _x;
    int _y;
    int _z;
};

} // namespace Engine

#endif // ENTITYPOSITIONMODEL_H