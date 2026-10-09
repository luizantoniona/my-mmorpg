#ifndef WORLDCOMBATACTION_H
#define WORLDCOMBATACTION_H

#include <MMORPGEngine/Entity/CombatActionEnum.h>
#include <MMORPGServer/Server/Runtime/World/System/WorldCombatAttackerEnum.h>

namespace Server {

class WorldRuntime;

class WorldCombatAction {
public:
    explicit WorldCombatAction( WorldRuntime& runtime );
    virtual ~WorldCombatAction();

    virtual Engine::CombatActionEnum id() const = 0;

    virtual void onTick() = 0;

    virtual bool start( WorldCombatAttackerEnum attacker, int idAttacker, int targetX, int targetY, int targetZ ) = 0;
    virtual void cancel( WorldCombatAttackerEnum attacker, int idAttacker ) = 0;
    virtual bool isActive( WorldCombatAttackerEnum attacker, int idAttacker ) const = 0;

protected:
    WorldRuntime& _runtime;
};

} // namespace Server

#endif // WORLDCOMBATACTION_H
