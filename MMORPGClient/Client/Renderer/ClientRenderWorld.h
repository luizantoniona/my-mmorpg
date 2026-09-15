#ifndef CLIENTRENDERWORLD_H
#define CLIENTRENDERWORLD_H

#include <QHash>

#include <MMORPGEngine/Renderer/World/RenderWorld.h>
#include <MMORPGEngine/World/WorldModel.h>

class ClientRenderWorld : public Engine::RenderWorld {
    Q_OBJECT
    Q_PROPERTY( Engine::WorldModel* world READ world WRITE setWorld )

public:
    explicit ClientRenderWorld( QObject* parent = nullptr );

    Engine::WorldModel* world() const;
    void setWorld( Engine::WorldModel* world );

    const Engine::WorldObjectModel* object( int x, int y, int z ) const override;

    const Engine::WorldTileModel* tile( int x, int y, int z ) const override;

    QList<Engine::RenderWorld::Entity> entities( int z ) const override;

    uint32_t width() const override;
    uint32_t height() const override;

    Q_INVOKABLE void setEntity( int idEntity, int x, int y, int z );
    Q_INVOKABLE void removeEntity( int idEntity );
    Q_INVOKABLE void clearEntities();

private:
    class EntityPosition {
    public:
        int x;
        int y;
        int z;
    };

    Engine::WorldModel* _world;
    QHash<int, EntityPosition> _entities;
};

#endif // CLIENTRENDERWORLD_H
