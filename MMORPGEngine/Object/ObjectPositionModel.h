#ifndef OBJECTPOSITIONMODEL_H
#define OBJECTPOSITIONMODEL_H

namespace Engine {

class ObjectPositionModel {
public:
    ObjectPositionModel();
    ~ObjectPositionModel();

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

#endif // OBJECTPOSITIONMODEL_H
