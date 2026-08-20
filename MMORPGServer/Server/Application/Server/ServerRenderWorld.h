#ifndef SERVERRENDERWORLD_H
#define SERVERRENDERWORLD_H

#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Renderer/World/RenderWorld.h>
#include <MMORPGEngine/World/Tile/TileModel.h>
#include <MMORPGEngine/World/WorldModel.h>

class ServerRenderWorld : public Engine::RenderWorld {
    Q_OBJECT

public:
    explicit ServerRenderWorld( QObject* parent = nullptr );

    Engine::WorldModel* world() const;
    void setWorld( Engine::WorldModel* world );

    const Engine::GroundModel* ground( uint32_t type ) const override;

    const Engine::TileModel* tile( int x, int y, int z ) const override;

private:
    Engine::DataManager& _data;
    Engine::WorldModel* _world;
};

#endif // SERVERRENDERWORLD_H
