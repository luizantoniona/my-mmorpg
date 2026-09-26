#ifndef ENTITYCOMBATMODEL_H
#define ENTITYCOMBATMODEL_H

namespace Engine {

class EntityCombatModel {
public:
    EntityCombatModel();

    double cooldownSeconds() const;
    void setCooldownSeconds( double cooldownSeconds );

    int counter() const;
    void setCounter( int counter );

    int cooldownTicks( int tickRate ) const;
    bool isReady( int tickRate ) const;

private:
    double _attackCooldownSeconds;
    int _attackCounter;
};

} // namespace Engine

#endif // ENTITYCOMBATMODEL_H
