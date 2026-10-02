#ifndef OWNCOMBATDTO_H
#define OWNCOMBATDTO_H

#include <set>

#include <json/json.h>

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/Entity/CombatActionEnum.h>

namespace Engine {

class OwnCombatDTO {
public:
    OwnCombatDTO();

    static OwnCombatDTO fromModel( const CharacterModel* character );
    static OwnCombatDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

    const std::set<CombatActionEnum>& actions() const;
    void setActions( const std::set<CombatActionEnum>& actions );

private:
    std::set<CombatActionEnum> _actions;
    int _idCharacter;
};

} // namespace Engine

#endif // OWNCOMBATDTO_H
