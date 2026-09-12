#ifndef RENDERWORLD_H
#define RENDERWORLD_H

#include <cstdint>

#include <QObject>

#include <MMORPGEngine/World/Object/WorldObjectModel.h>
#include <MMORPGEngine/World/Tile/WorldTileModel.h>

namespace Engine {

class RenderWorld : public QObject {
    Q_OBJECT

public:
    explicit RenderWorld( QObject* parent = nullptr );

    virtual const WorldObjectModel* object( int x, int y, int z ) const = 0;

    virtual const WorldTileModel* tile( int x, int y, int z ) const = 0;

    virtual uint32_t width() const = 0;
    virtual uint32_t height() const = 0;

    Q_INVOKABLE bool hasObject( int x, int y, int z ) const;
    Q_INVOKABLE bool hasTile( int x, int y, int z ) const;

signals:
    void boundsChanged();
};

} // namespace Engine

#endif // RENDERWORLD_H
