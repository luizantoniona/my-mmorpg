#ifndef ENTITYVITALSDTO_H
#define ENTITYVITALSDTO_H

#include <json/json.h>

namespace Engine {

class EntityVitalsDTO {
public:
    EntityVitalsDTO();
    ~EntityVitalsDTO();

    static EntityVitalsDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

    double health() const;
    void setHealth( double health );

    double maxHealth() const;
    void setMaxHealth( double maxHealth );

    double mana() const;
    void setMana( double mana );

    double maxMana() const;
    void setMaxMana( double maxMana );

    double stamina() const;
    void setStamina( double stamina );

    double maxStamina() const;
    void setMaxStamina( double maxStamina );

private:
    int _idCharacter;
    double _health;
    double _maxHealth;
    double _mana;
    double _maxMana;
    double _stamina;
    double _maxStamina;
};

} // namespace Engine

#endif // ENTITYVITALSDTO_H
