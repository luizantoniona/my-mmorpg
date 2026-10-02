#ifndef ENTITYCOMBATMODEL_H
#define ENTITYCOMBATMODEL_H

#include <set>

#include <MMORPGEngine/Entity/CombatActionEnum.h>

namespace Engine {

class EntityCombatModel {
public:
    EntityCombatModel();

    bool isActionAvailable( CombatActionEnum action ) const;
    void setActionAvailable( CombatActionEnum action, bool available );

    const std::set<CombatActionEnum>& availableActions() const;
    void setAvailableActions( const std::set<CombatActionEnum>& availableActions );

    double cooldownSeconds() const;
    void setCooldownSeconds( double cooldownSeconds );

    int attackRange() const;
    void setAttackRange( int attackRange );

    int counter() const;
    void setCounter( int counter );

    int cooldownTicks( int tickRate ) const;
    bool isReady( int tickRate ) const;

private:
    std::set<CombatActionEnum> _availableActions;
    double _attackCooldownSeconds;
    int _attackCounter;
    int _attackRange;
};

} // namespace Engine

#endif // ENTITYCOMBATMODEL_H
