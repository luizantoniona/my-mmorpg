#ifndef SERVERRENDERWORLD_H
#define SERVERRENDERWORLD_H

#include <MMORPGEngine/Renderer/World/RenderWorld.h>
#include <MMORPGEngine/World/Tile/TileModel.h>
#include <MMORPGEngine/World/WorldModel.h>

class ServerRenderWorld : public Engine::RenderWorld {
    Q_OBJECT

public:
    explicit ServerRenderWorld( Engine::WorldModel* world, QObject* parent = nullptr );

    const Engine::TileModel* tile( int x, int y, int z ) const override;

private:
    Engine::WorldModel* _world;
};

#endif // SERVERRENDERWORLD_H
