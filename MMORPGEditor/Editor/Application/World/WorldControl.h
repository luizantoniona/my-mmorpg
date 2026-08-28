#ifndef WORLDCONTROL_H
#define WORLDCONTROL_H

#include <memory>

#include <QObject>

#include <MMORPGEngine/World/WorldModel.h>

class WorldControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( QString worldName READ worldName NOTIFY worldChanged )
    Q_PROPERTY( int worldWidth READ worldWidth NOTIFY worldChanged )
    Q_PROPERTY( int worldHeight READ worldHeight NOTIFY worldChanged )
    Q_PROPERTY( Engine::WorldModel* world READ world CONSTANT )

public:
    explicit WorldControl( QObject* parent = nullptr );

    QString worldName() const;
    int worldWidth() const;
    int worldHeight() const;

    Engine::WorldModel* world() const;

public slots:
    void loadWorld();

signals:
    void worldChanged();

private:
    std::unique_ptr<Engine::WorldModel> _world;
};

#endif // WORLDCONTROL_H
