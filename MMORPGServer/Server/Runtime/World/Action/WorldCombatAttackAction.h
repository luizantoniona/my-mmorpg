#ifndef WORLDCOMBATATTACKACTION_H
#define WORLDCOMBATATTACKACTION_H

#include <vector>

#include <MMORPGServer/Server/Runtime/World/Action/WorldCombatAction.h>

namespace Server {

class WorldCombatAttackAction : public WorldCombatAction {
public:
    explicit WorldCombatAttackAction( WorldRuntime& runtime );

    Engine::CombatActionEnum id() const override;

    void onTick() override;

    bool start( WorldCombatAttackerEnum attacker, int idAttacker, int targetX, int targetY, int targetZ ) override;
    void cancel( WorldCombatAttackerEnum attacker, int idAttacker ) override;
    bool isActive( WorldCombatAttackerEnum attacker, int idAttacker ) const override;

    bool resolveCharacterAttack( int idCharacter, int x, int y, int z, double damage );
    void resolveCreatureAttack( int idCreature, int x, int y, int z, double damage );

private:
    class PendingAttack {
    public:
        PendingAttack( WorldCombatAttackerEnum attacker, int idAttacker, int x, int y, int z, double damage, int ticksRemaining ) :
            damage( damage ),
            attacker( attacker ),
            idAttacker( idAttacker ),
            x( x ),
            y( y ),
            z( z ),
            ticksRemaining( ticksRemaining ) {
        }

        double damage;
        WorldCombatAttackerEnum attacker;
        int idAttacker;
        int x;
        int y;
        int z;
        int ticksRemaining;
    };

private:
    bool startCharacterAttack( int idCharacter, int targetX, int targetY, int targetZ );
    bool startCreatureAttack( int idCreature, int targetX, int targetY, int targetZ );
    int castTicks() const;

private:
    std::vector<PendingAttack> _pendingAttacks;
};

} // namespace Server

#endif // WORLDCOMBATATTACKACTION_H
