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

    std::vector<int> floors() const override;

    uint32_t width() const override;
    uint32_t height() const override;

    Q_INVOKABLE void setOwnCharacter( int idCharacter, int x, int y, int z );

    Q_INVOKABLE void addCharacter( int idCharacter, int x, int y, int z );
    Q_INVOKABLE void removeCharacter( int idCharacter );

    Q_INVOKABLE void addCreature( int idCreature, int x, int y, int z );
    Q_INVOKABLE void removeCreature( int idCreature );

    Q_INVOKABLE void clearEntities();

private:
    class EntityPosition {
    public:
        int x;
        int y;
        int z;
    };

    Engine::WorldModel* _world;
    QHash<int, EntityPosition> _ownCharacter;
    QHash<int, EntityPosition> _characters;
    QHash<int, EntityPosition> _creatures;
};

#endif // CLIENTRENDERWORLD_H
