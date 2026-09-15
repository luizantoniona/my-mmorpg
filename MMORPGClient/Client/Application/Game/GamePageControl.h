#ifndef GAMEPAGECONTROL_H
#define GAMEPAGECONTROL_H

#include <memory>

#include <QObject>
#include <QVariantList>

#include <MMORPGEngine/Network/WebSocketClient.h>
#include <MMORPGEngine/World/WorldModel.h>

class GamePageControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( Engine::WorldModel* world READ world CONSTANT )
    Q_PROPERTY( QVariantList floors READ floors NOTIFY worldChanged )
    Q_PROPERTY( QString worldName READ worldName NOTIFY worldChanged )
    Q_PROPERTY( int worldWidth READ worldWidth NOTIFY worldChanged )
    Q_PROPERTY( int worldHeight READ worldHeight NOTIFY worldChanged )
    Q_PROPERTY( int spawnFloor READ spawnFloor NOTIFY worldEntryReceived )
    Q_PROPERTY( int spawnX READ spawnX NOTIFY worldEntryReceived )
    Q_PROPERTY( int spawnY READ spawnY NOTIFY worldEntryReceived )

public:
    explicit GamePageControl( QObject* parent = nullptr );
    ~GamePageControl();

    Engine::WorldModel* world() const;

    QVariantList floors() const;

    QString worldName() const;

    int worldWidth() const;
    int worldHeight() const;

    int spawnFloor() const;
    int spawnX() const;
    int spawnY() const;

public slots:
    void loadWorld();
    void connectToWorld( int idCharacter );

    void move( int dx, int dy );

signals:
    void worldChanged();

    void worldEntryReceived();
    void worldEntryFailed( const QString& error );

private:
    void onMessageReceived( const QString& message );

private:
    std::unique_ptr<Engine::WorldModel> _world;
    Engine::WebSocketClient _webSocket;

    int _spawnFloor;
    int _spawnX;
    int _spawnY;
};

#endif // GAMEPAGECONTROL_H
