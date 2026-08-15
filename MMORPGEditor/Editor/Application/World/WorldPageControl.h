#ifndef WORLDPAGECONTROL_H
#define WORLDPAGECONTROL_H

#include <memory>

#include <QObject>

#include <MMORPGEngine/World/WorldModel.h>

class WorldPageControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( QString worldName READ worldName NOTIFY worldChanged )
    Q_PROPERTY( int worldWidth READ worldWidth NOTIFY worldChanged )
    Q_PROPERTY( int worldHeight READ worldHeight NOTIFY worldChanged )

public:
    explicit WorldPageControl( QObject* parent = nullptr );

    QString worldName() const;
    int worldWidth() const;
    int worldHeight() const;

public slots:
    void loadWorld();

signals:
    void worldChanged();

private:
    std::unique_ptr<Engine::WorldModel> _world;
};

#endif // WORLDPAGECONTROL_H
