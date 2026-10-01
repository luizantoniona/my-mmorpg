#ifndef WORLDCOMBATSYSTEM_H
#define WORLDCOMBATSYSTEM_H

#include <vector>

#include <MMORPGServer/Server/Runtime/World/System/WorldCombatAttackerEnum.h>
#include <MMORPGServer/Server/Runtime/World/System/WorldSystem.h>

namespace Server {

class WorldCombatSystem : public WorldSystem {
public:
    explicit WorldCombatSystem( WorldRuntime& runtime );

    void onTick() override;

    bool attack( int idCharacter, int dx, int dy );
    bool attackCreature( int idCreature, int x, int y, int z );

    void cancelCharacterAttack( int idCharacter );
    bool hasPendingCreatureAttack( int idCreature ) const;

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
    bool hasPendingAttack( WorldCombatAttackerEnum attacker, int idAttacker ) const;
    int castTicks() const;

private:
    std::vector<PendingAttack> _pendingAttacks;
};

} // namespace Server

#endif // WORLDCOMBATSYSTEM_H
