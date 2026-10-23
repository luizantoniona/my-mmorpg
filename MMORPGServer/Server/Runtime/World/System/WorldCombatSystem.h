#ifndef WORLDCOMBATSYSTEM_H
#define WORLDCOMBATSYSTEM_H

#include <memory>
#include <vector>

#include <MMORPGServer/Server/Runtime/World/Action/WorldCombatAction.h>
#include <MMORPGServer/Server/Runtime/World/Action/WorldCombatAttackAction.h>
#include <MMORPGServer/Server/Runtime/World/Command/Character/CharacterAttackEntityCommand.h>
#include <MMORPGServer/Server/Runtime/World/System/WorldCombatAttackerEnum.h>
#include <MMORPGServer/Server/Runtime/World/System/WorldSystem.h>

namespace Server {

class WorldCombatSystem : public WorldSystem {
public:
    explicit WorldCombatSystem( WorldRuntime& runtime );

    void onTick() override;

    void handle( const CharacterAttackEntityCommand& command );

    bool attack( int idCharacter, int dx, int dy );
    bool attackCreature( int idCreature, int x, int y, int z );

    void cancelCharacterActions( int idCharacter );
    bool hasPendingCreatureAttack( int idCreature ) const;

    bool resolveCharacterAttack( int idCharacter, int x, int y, int z, double damage );
    void resolveCreatureAttack( int idCreature, int x, int y, int z, double damage );

private:
    bool perform( Engine::CombatActionEnum action, WorldCombatAttackerEnum attacker, int idAttacker, int targetX, int targetY, int targetZ );

private:
    std::vector<std::unique_ptr<WorldCombatAction>> _actions;
    WorldCombatAttackAction* _attackAction;
};

} // namespace Server

#endif // WORLDCOMBATSYSTEM_H
