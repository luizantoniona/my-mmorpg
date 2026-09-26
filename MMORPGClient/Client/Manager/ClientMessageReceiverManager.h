#ifndef CLIENTMESSAGERECEIVERMANAGER_H
#define CLIENTMESSAGERECEIVERMANAGER_H

#include <QObject>
#include <QString>

#include <MMORPGEngine/Entity/Character/OwnCharacterDTO.h>
#include <MMORPGEngine/Entity/Character/OwnEquipmentDTO.h>
#include <MMORPGEngine/Entity/Character/OwnInventoryDTO.h>

class ClientMessageReceiverManager : public QObject {
    Q_OBJECT

public:
    explicit ClientMessageReceiverManager( QObject* parent = nullptr );
    ~ClientMessageReceiverManager();

public slots:
    void receiveMessage( const QString& message );

signals:
    void errorReceived( const QString& error );

    void ownCharacterReceived( const Engine::OwnCharacterDTO& state );
    void ownEquipmentReceived( const Engine::OwnEquipmentDTO& state );
    void ownInventoryReceived( const Engine::OwnInventoryDTO& state );

    void characterStateReceived( int idCharacter, int x, int y, int z );
    void creatureStateReceived( int idCreature, int x, int y, int z );
    void entityLeftReceived( int idCharacter );
};

#endif // CLIENTMESSAGERECEIVERMANAGER_H
