#ifndef GAMEPAGECONTROL_H
#define GAMEPAGECONTROL_H

#include <memory>

#include <QObject>
#include <QVariantList>

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
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
    Q_PROPERTY( double health READ health NOTIFY vitalsChanged )
    Q_PROPERTY( double maxHealth READ maxHealth NOTIFY vitalsChanged )
    Q_PROPERTY( double mana READ mana NOTIFY vitalsChanged )
    Q_PROPERTY( double maxMana READ maxMana NOTIFY vitalsChanged )
    Q_PROPERTY( double stamina READ stamina NOTIFY vitalsChanged )
    Q_PROPERTY( double maxStamina READ maxStamina NOTIFY vitalsChanged )

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

    double health() const;
    double maxHealth() const;
    double mana() const;
    double maxMana() const;
    double stamina() const;
    double maxStamina() const;

public slots:
    void loadWorld();
    void connectToWorld( int idCharacter );

    void move( int dx, int dy );

signals:
    void worldChanged();

    void worldEntryReceived();
    void worldEntryFailed( const QString& error );

    void entityStateReceived( int idCharacter, int x, int y, int z, const QString& orientation );
    void entityLeftReceived( int idCharacter );

    void vitalsChanged();

private:
    void onMessageReceived( const QString& message );

private:
    std::unique_ptr<Engine::WorldModel> _world;
    Engine::WebSocketClient _webSocket;

    Engine::CharacterModel _character;
};

#endif // GAMEPAGECONTROL_H
