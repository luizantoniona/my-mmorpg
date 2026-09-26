#ifndef ENTITYCOMBATMODEL_H
#define ENTITYCOMBATMODEL_H

namespace Engine {

class EntityCombatModel {
public:
    EntityCombatModel();

    int cooldown() const;
    void setCooldown( int cooldown );

    int counter() const;
    void setCounter( int counter );

    bool isReady() const;

private:
    int _attackCooldown;
    int _attackCounter;
};

} // namespace Engine

#endif // ENTITYCOMBATMODEL_H
