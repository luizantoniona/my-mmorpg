#ifndef GAMEPAGECONTROL_H
#define GAMEPAGECONTROL_H

#include <memory>

#include <QObject>
#include <QVariantList>

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/Entity/Character/OwnCharacterDTO.h>
#include <MMORPGEngine/Network/WebSocketClient.h>
#include <MMORPGEngine/World/WorldModel.h>

class GamePageControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( Engine::WorldModel* world READ world CONSTANT )
    Q_PROPERTY( QVariantList floors READ floors NOTIFY worldChanged )
    Q_PROPERTY( QString worldName READ worldName NOTIFY worldChanged )
    Q_PROPERTY( int worldWidth READ worldWidth NOTIFY worldChanged )
    Q_PROPERTY( int worldHeight READ worldHeight NOTIFY worldChanged )
    Q_PROPERTY( double health READ health NOTIFY vitalsChanged )
    Q_PROPERTY( double maxHealth READ maxHealth NOTIFY vitalsChanged )
    Q_PROPERTY( double mana READ mana NOTIFY vitalsChanged )
    Q_PROPERTY( double maxMana READ maxMana NOTIFY vitalsChanged )
    Q_PROPERTY( double stamina READ stamina NOTIFY vitalsChanged )
    Q_PROPERTY( double maxStamina READ maxStamina NOTIFY vitalsChanged )
    Q_PROPERTY( int characterX READ characterX NOTIFY positionChanged )
    Q_PROPERTY( int characterY READ characterY NOTIFY positionChanged )
    Q_PROPERTY( int characterZ READ characterZ NOTIFY positionChanged )

public:
    explicit GamePageControl( QObject* parent = nullptr );
    ~GamePageControl();

    Engine::WorldModel* world() const;

    QVariantList floors() const;

    QString worldName() const;

    int worldWidth() const;
    int worldHeight() const;

    double health() const;
    double maxHealth() const;
    double mana() const;
    double maxMana() const;
    double stamina() const;
    double maxStamina() const;

    int characterX() const;
    int characterY() const;
    int characterZ() const;

public slots:
    void loadWorld();
    void connectToWorld( int idCharacter );

    void move( int dx, int dy );
    void attack( int dx, int dy );

    void leaveWorld();

signals:
    void worldChanged();
    void vitalsChanged();
    void positionChanged();

    void worldEntryReceived();
    void worldEntryFailed( const QString& error );

    void characterEventAttackReceived( int idCharacter, int x, int y, int z );

    void worldLeft();

private slots:
    void onOwnCharacterReceived( const Engine::OwnCharacterDTO& state );

private:
    std::unique_ptr<Engine::WorldModel> _world;
    Engine::CharacterModel _character;
    Engine::WebSocketClient _webSocket;
};

#endif // GAMEPAGECONTROL_H
