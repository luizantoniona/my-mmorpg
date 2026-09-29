#include "AttackCharacterCommand.h"

#include <QDebug>

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace {

constexpr double ATTACK_DAMAGE = 5.0;
constexpr double ATTACK_STAMINA_COST = 10.0;

} // namespace

namespace Server {

AttackCharacterCommand::AttackCharacterCommand( int idCharacter, int dx, int dy ) :
    _idCharacter( idCharacter ),
    _dx( dx ),
    _dy( dy ) {
}

void AttackCharacterCommand::execute( WorldRuntime& runtime ) {
    Engine::CharacterModel* character = runtime.character( _idCharacter );
    if ( !character ) {
        return;
    }

    const Engine::EntityPositionModel currentPosition = character->position();
    const int targetX = currentPosition.x() + _dx;
    const int targetY = currentPosition.y() + _dy;
    const int z = currentPosition.z();

    const bool hasStamina = character->vitals().stamina() >= ATTACK_STAMINA_COST;
    const bool attackDue = character->combat().isReady( runtime.tickRate() );

    if ( !hasStamina || !attackDue ) {
        qInfo() << "[AttackCharacterCommand] Attack blocked [CHARACTER]" << _idCharacter << "[HAS_STAMINA]" << hasStamina
                << "[STAMINA]" << character->vitals().stamina() << "[ATTACK_DUE]" << attackDue;
        return;
    }

    runtime.attackTile( _idCharacter, targetX, targetY, z, ATTACK_DAMAGE, ATTACK_STAMINA_COST );

    qInfo() << "[AttackCharacterCommand] Character attacked tile [CHARACTER]" << _idCharacter << "[X]" << targetX << "[Y]" << targetY << "[Z]" << z;
}

} // namespace Server
