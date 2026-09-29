#ifndef SERVERMESSAGERECEIVER_H
#define SERVERMESSAGERECEIVER_H

#include <QObject>
#include <QString>

#include <MMORPGEngine/Entity/Character/OwnCharacterDTO.h>
#include <MMORPGEngine/Entity/Character/OwnEquipmentDTO.h>
#include <MMORPGEngine/Entity/Character/OwnInventoryDTO.h>

namespace Engine {

class ServerMessageReceiver : public QObject {
    Q_OBJECT

public:
    explicit ServerMessageReceiver( QObject* parent = nullptr );
    ~ServerMessageReceiver();

public slots:
    void receiveMessage( const QString& message );

signals:
    void errorReceived( const QString& error );

    void ownCharacterReceived( const Engine::OwnCharacterDTO& state );
    void ownEquipmentReceived( const Engine::OwnEquipmentDTO& state );
    void ownInventoryReceived( const Engine::OwnInventoryDTO& state );

    void characterStateReceived( int idCharacter, int x, int y, int z, double movementSeconds );
    void creatureStateReceived( int idCreature, int x, int y, int z, double movementSeconds );
    void characterEventAttackReceived( int idCharacter, int x, int y, int z );
    void characterEventLeaveReceived( int idCharacter );
    void creatureEventLeaveReceived( int idCreature );
};

} // namespace Engine

#endif // SERVERMESSAGERECEIVER_H
