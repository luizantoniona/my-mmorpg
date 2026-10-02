#ifndef CHARACTERINTENTACTIONDTO_H
#define CHARACTERINTENTACTIONDTO_H

#include <optional>

#include <json/json.h>

#include <MMORPGEngine/Entity/CombatActionEnum.h>

namespace Engine {

class CharacterIntentActionDTO {
public:
    CharacterIntentActionDTO();
    ~CharacterIntentActionDTO();

    static CharacterIntentActionDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    const std::optional<CombatActionEnum>& action() const;
    void setAction( CombatActionEnum action );

    int dx() const;
    void setDx( int dx );

    int dy() const;
    void setDy( int dy );

private:
    std::optional<CombatActionEnum> _action;
    int _dx;
    int _dy;
};

} // namespace Engine

#endif // CHARACTERINTENTACTIONDTO_H
