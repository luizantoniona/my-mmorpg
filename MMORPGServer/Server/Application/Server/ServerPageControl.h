#ifndef SERVERPAGECONTROL_H
#define SERVERPAGECONTROL_H

#include <memory>

#include <QObject>
#include <QVariantList>

#include <MMORPGEngine/Network/WebSocket/ServerMessageReceiver.h>
#include <MMORPGServer/Server/Network/Observer/GodObserver.h>

namespace Server {

class ServerPageControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( QString worldName READ worldName CONSTANT )
    Q_PROPERTY( int worldWidth READ worldWidth CONSTANT )
    Q_PROPERTY( int worldHeight READ worldHeight CONSTANT )
    Q_PROPERTY( QVariantList floors READ floors CONSTANT )

public:
    explicit ServerPageControl( QObject* parent = nullptr );
    ~ServerPageControl();

    QString worldName() const;
    int worldWidth() const;
    int worldHeight() const;
    QVariantList floors() const;

    Q_INVOKABLE QVariantList connectedCharacters() const;
    Q_INVOKABLE void disconnectCharacter( int idCharacter );

signals:
    void characterStateReceived( int idCharacter, int x, int y, int z, double movementSeconds );
    void creatureStateReceived( int idCreature, int x, int y, int z, double movementSeconds );

    void entityAttackedReceived( int idCharacter, int x, int y, int z );

    void entityLeftReceived( int idCharacter );
    void creatureLeftReceived( int idCreature );

private:
    std::shared_ptr<GodObserver> _godObserver;
    Engine::ServerMessageReceiver _messageReceiver;
};

} // namespace Server

#endif // SERVERPAGECONTROL_H
