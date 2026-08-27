#ifndef EDITORRENDERWORLD_H
#define EDITORRENDERWORLD_H

#include <MMORPGEngine/Renderer/World/RenderWorld.h>
#include <MMORPGEngine/World/WorldModel.h>

class EditorRenderWorld : public Engine::RenderWorld {
    Q_OBJECT
    Q_PROPERTY( Engine::WorldModel* world READ world WRITE setWorld )

public:
    explicit EditorRenderWorld( QObject* parent = nullptr );

    Engine::WorldModel* world() const;
    void setWorld( Engine::WorldModel* world );

    const Engine::WorldObjectModel* object( int x, int y, int z ) const override;

    const Engine::WorldTileModel* tile( int x, int y, int z ) const override;

private:
    Engine::WorldModel* _world;
};

#endif // EDITORRENDERWORLD_H
