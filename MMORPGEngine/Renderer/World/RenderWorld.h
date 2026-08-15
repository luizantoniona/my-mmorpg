#ifndef RENDERWORLD_H
#define RENDERWORLD_H

#include <QObject>

#include <MMORPGEngine/World/Tile/TileModel.h>

namespace Engine {

class RenderWorld : public QObject {
    Q_OBJECT

public:
    explicit RenderWorld( QObject* parent = nullptr );

    virtual const TileModel* tile( int x, int y, int z ) const = 0;
};

} // namespace Engine

#endif // RENDERWORLD_H
