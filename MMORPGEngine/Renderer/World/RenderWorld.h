#ifndef RENDERWORLD_H
#define RENDERWORLD_H

#include <QObject>

#include <MMORPGEngine/Data/Ground/GroundModel.h>
#include <MMORPGEngine/World/Tile/TileModel.h>

namespace Engine {

class RenderWorld : public QObject {
    Q_OBJECT

public:
    explicit RenderWorld( QObject* parent = nullptr );

    virtual const GroundModel* ground( uint32_t id ) const = 0;

    virtual const TileModel* tile( int x, int y, int z ) const = 0;
};

} // namespace Engine

#endif // RENDERWORLD_H
