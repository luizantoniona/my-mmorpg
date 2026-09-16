#ifndef SERVERRENDERWORLD_H
#define SERVERRENDERWORLD_H

#include <MMORPGEngine/Renderer/World/RenderWorld.h>
#include <MMORPGEngine/World/WorldModel.h>

class ServerRenderWorld : public Engine::RenderWorld {
    Q_OBJECT

public:
    explicit ServerRenderWorld( QObject* parent = nullptr );

    Engine::WorldModel* world() const;
    void setWorld( Engine::WorldModel* world );

    const Engine::WorldObjectModel* object( int x, int y, int z ) const override;

    const Engine::WorldTileModel* tile( int x, int y, int z ) const override;

    QList<Engine::RenderWorld::Entity> entities( int z ) const override;

    std::vector<int> floors() const override;

    uint32_t width() const override;
    uint32_t height() const override;

private:
    Engine::WorldModel* _world;
};

#endif // SERVERRENDERWORLD_H
