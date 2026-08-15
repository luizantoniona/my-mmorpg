#ifndef SERVERRENDERWORLD_H
#define SERVERRENDERWORLD_H

#include <QMap>

#include <MMORPGEngine/Renderer/World/RenderWorld.h>
#include <MMORPGEngine/World/Tile/TileModel.h>

class ServerRenderWorld : public Engine::RenderWorld {
    Q_OBJECT

public:
    explicit ServerRenderWorld( QObject* parent = nullptr );

    const Engine::TileModel* tile( int x, int y, int z ) const override;

private:
    QString key( int x, int y, int z ) const;

private:
    QMap<QString, Engine::TileModel> _tiles;
};

#endif // SERVERRENDERWORLD_H
