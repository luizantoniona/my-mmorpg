#ifndef SERVERMESSAGERECEIVER_H
#define SERVERMESSAGERECEIVER_H

#include <cstdint>
#include <vector>

#include <QObject>
#include <QString>

#include <MMORPGEngine/Entity/AttackStrikeDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterAppearanceModel.h>
#include <MMORPGEngine/Entity/Character/OwnCharacterDTO.h>
#include <MMORPGEngine/Entity/Character/OwnCombatDTO.h>
#include <MMORPGEngine/Entity/Character/OwnEquipmentDTO.h>
#include <MMORPGEngine/Entity/Character/OwnInventoryDTO.h>
#include <MMORPGEngine/World/Chunk/ChunkDTO.h>

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
    void ownCombatReceived( const Engine::OwnCombatDTO& state );
    void ownEquipmentReceived( const Engine::OwnEquipmentDTO& state );
    void ownInventoryReceived( const Engine::OwnInventoryDTO& state );

    void characterStateReceived( int idCharacter, int x, int y, int z, double movementSeconds );
    void characterAppearanceReceived( int idCharacter, const Engine::CharacterAppearanceModel& appearance );
    void creatureStateReceived( int idCreature, int x, int y, int z, double movementSeconds, int footprint, const QString& creatureType );
    void characterEventAttackStartReceived( int idCharacter, int x, int y, int z, double castSeconds );
    void characterEventAttackReceived( int idCharacter, int x, int y, int z );
    void characterEventLeaveReceived( int idCharacter );
    void creatureEventAttackStartReceived( int idCreature, int x, int y, int z, double castSeconds );
    void creatureEventAttackReceived( int idCreature, int x, int y, int z );
    void creatureEventLeaveReceived( int idCreature );
    void attackWarningReceived( int z, int x, int y, double seconds );
    void groundItemStateReceived( int idGroundItem, const QString& idItem, int x, int y, int z );
    void groundItemEventLeaveReceived( int idGroundItem );
    void chunkReceived( const Engine::ChunkDTO& chunk );
    void chunkEventLeaveReceived( int chunkX, int chunkY );

private:
    void emitAttackWarnings( const std::vector<AttackStrikeDTO>& strikes, int targetX, int targetY, int z, double castSeconds );
};

} // namespace Engine

#endif // SERVERMESSAGERECEIVER_H
