#ifndef WORLDCOMBATATTACKACTION_H
#define WORLDCOMBATATTACKACTION_H

#include <utility>
#include <vector>

#include <json/json.h>

#include <MMORPGEngine/Data/Attack/AttackHitModeEnum.h>
#include <MMORPGServer/Server/Runtime/World/Action/WorldCombatAction.h>
#include <MMORPGServer/Server/Runtime/World/Action/WorldCombatStrikeModel.h>

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
    bool resolveCharacterStrike( int idCharacter, const std::vector<std::pair<int, int>>& cells, int z, Engine::AttackHitModeEnum hitMode, double damage );
    void resolveCreatureAttack( int idCreature, int x, int y, int z, double damage );
    void resolveCreatureStrike( int idCreature, const std::vector<std::pair<int, int>>& cells, int z, Engine::AttackHitModeEnum hitMode, double damage );

private:
    class PendingAttack {
    public:
        PendingAttack( WorldCombatAttackerEnum attacker, int idAttacker, std::vector<std::pair<int, int>> cells, int z, Engine::AttackHitModeEnum hitMode, double damage, int ticksRemaining ) :
            cells( std::move( cells ) ),
            damage( damage ),
            attacker( attacker ),
            hitMode( hitMode ),
            idAttacker( idAttacker ),
            z( z ),
            ticksRemaining( ticksRemaining ) {
        }

        std::vector<std::pair<int, int>> cells;
        double damage;
        WorldCombatAttackerEnum attacker;
        Engine::AttackHitModeEnum hitMode;
        int idAttacker;
        int z;
        int ticksRemaining;
    };

private:
    bool startCharacterAttack( int idCharacter, int targetX, int targetY, int targetZ );
    bool startCreatureAttack( int idCreature, int targetX, int targetY, int targetZ );
    static Json::Value cellsPayload( const std::vector<std::pair<int, int>>& cells );
    static Json::Value strikePayload( const WorldCombatStrikeModel& strike );

private:
    std::vector<PendingAttack> _pendingAttacks;
};

} // namespace Server

#endif // WORLDCOMBATATTACKACTION_H
