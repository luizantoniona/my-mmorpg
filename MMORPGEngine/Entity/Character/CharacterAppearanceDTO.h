#ifndef CHARACTERAPPEARANCEDTO_H
#define CHARACTERAPPEARANCEDTO_H

#include <json/json.h>

#include <MMORPGEngine/Entity/Character/CharacterAppearanceModel.h>
#include <MMORPGEngine/Entity/Character/CharacterModel.h>

namespace Engine {

class CharacterAppearanceDTO {
public:
    CharacterAppearanceDTO();
    ~CharacterAppearanceDTO();

    static CharacterAppearanceDTO fromModel( const CharacterModel* character );
    static CharacterAppearanceDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    static Json::Value selectionToJson( const CharacterAppearanceModel& appearance );
    static bool selectionFromJson( const Json::Value& json, CharacterAppearanceModel& appearance );

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

    const CharacterAppearanceModel& appearance() const;
    void setAppearance( const CharacterAppearanceModel& appearance );

private:
    CharacterAppearanceModel _appearance;
    int _idCharacter;
};

} // namespace Engine

#endif // CHARACTERAPPEARANCEDTO_H
