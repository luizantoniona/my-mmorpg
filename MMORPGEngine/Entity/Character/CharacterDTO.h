#ifndef CHARACTERDTO_H
#define CHARACTERDTO_H

#include <json/json.h>

#include <MMORPGEngine/Entity/EntityOrientationEnum.h>

namespace Engine {

class CharacterDTO {
public:
    CharacterDTO();
    ~CharacterDTO();

    static CharacterDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

    int x() const;
    void setX( int x );

    int y() const;
    void setY( int y );

    int z() const;
    void setZ( int z );

    EntityOrientationEnum orientation() const;
    void setOrientation( EntityOrientationEnum orientation );

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
    int _x;
    int _y;
    int _z;
    EntityOrientationEnum _orientation;
};

} // namespace Engine

#endif // CHARACTERDTO_H
