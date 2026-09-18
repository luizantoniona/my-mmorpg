#ifndef WORLDCONTROL_H
#define WORLDCONTROL_H

#include <memory>

#include <QObject>
#include <QVariantList>

#include <MMORPGEngine/World/WorldModel.h>

class WorldControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( Engine::WorldModel* world READ world CONSTANT )
    Q_PROPERTY( QVariantList floors READ floors NOTIFY worldChanged )
    Q_PROPERTY( QString worldName READ worldName NOTIFY worldChanged )
    Q_PROPERTY( int worldWidth READ worldWidth NOTIFY worldChanged )
    Q_PROPERTY( int worldHeight READ worldHeight NOTIFY worldChanged )

public:
    explicit WorldControl( QObject* parent = nullptr );

    Engine::WorldModel* world() const;

    QVariantList floors() const;

    QString worldName() const;

    int worldWidth() const;
    int worldHeight() const;

public slots:
    void loadWorld();
    bool saveWorld();

    bool addFloor( bool above );
    bool removeFloor( int z );

    void paintTile( int x, int y, int z, int tileType );
    void paintObject( int x, int y, int z, int objectType );

signals:
    void worldChanged();

private:
    std::unique_ptr<Engine::WorldModel> _world;
};

#endif // WORLDCONTROL_H
