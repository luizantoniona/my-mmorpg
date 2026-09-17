#ifndef OWNCHARACTERVITALSDTO_H
#define OWNCHARACTERVITALSDTO_H

#include <json/json.h>

namespace Engine {

class OwnCharacterVitalsDTO {
public:
    OwnCharacterVitalsDTO();
    ~OwnCharacterVitalsDTO();

    static OwnCharacterVitalsDTO fromJson( const Json::Value& json );
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
    double _health;
    double _maxHealth;
    double _mana;
    double _maxMana;
    double _stamina;
    double _maxStamina;
    int _idCharacter;
};

} // namespace Engine

#endif // OWNCHARACTERVITALSDTO_H
