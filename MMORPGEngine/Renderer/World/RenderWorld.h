#ifndef RENDERWORLD_H
#define RENDERWORLD_H

#include <QObject>

#include <MMORPGEngine/Data/Object/ObjectTextureModel.h>
#include <MMORPGEngine/Data/Tile/TileTextureModel.h>
#include <MMORPGEngine/Object/ObjectModel.h>
#include <MMORPGEngine/World/Tile/TileModel.h>

namespace Engine {

class RenderWorld : public QObject {
    Q_OBJECT

public:
    explicit RenderWorld( QObject* parent = nullptr );

    virtual const TileModel* tile( int x, int y, int z ) const = 0;
    virtual const TileTextureModel* tileTexture( uint32_t id ) const = 0;

    virtual const ObjectModel* object( int x, int y, int z ) const = 0;
    virtual const ObjectTextureModel* objectTexture( uint32_t id ) const = 0;
};

} // namespace Engine

#endif // RENDERWORLD_H
