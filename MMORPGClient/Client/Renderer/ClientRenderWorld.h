#ifndef CLIENTRENDERWORLD_H
#define CLIENTRENDERWORLD_H

#include <MMORPGEngine/Entity/Character/OwnCharacterDTO.h>
#include <MMORPGEngine/Renderer/World/StreamRenderWorld.h>
#include <MMORPGEngine/World/WorldModel.h>

class ClientRenderWorld : public Engine::StreamRenderWorld {
    Q_OBJECT
    Q_PROPERTY( Engine::WorldModel* world READ world WRITE setWorld )

public:
    explicit ClientRenderWorld( QObject* parent = nullptr );

    Engine::WorldModel* world() const;
    void setWorld( Engine::WorldModel* world );

    const Engine::WorldObjectModel* object( int x, int y, int z ) const override;

    const Engine::WorldTileModel* tile( int x, int y, int z ) const override;

    std::vector<int> floors() const override;

    uint32_t width() const override;
    uint32_t height() const override;

private slots:
    void onOwnCharacterReceived( const Engine::OwnCharacterDTO& state );

private:
    Engine::WorldModel* _world;
};

#endif // CLIENTRENDERWORLD_H
