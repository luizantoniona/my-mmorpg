#ifndef CREATURETYPEVITALSMODEL_H
#define CREATURETYPEVITALSMODEL_H

namespace Engine {

class CreatureTypeVitalsModel {
public:
    CreatureTypeVitalsModel();

    double maxHealth() const;
    void setMaxHealth( double maxHealth );

private:
    double _maxHealth;
};

} // namespace Engine

#endif // CREATURETYPEVITALSMODEL_H
