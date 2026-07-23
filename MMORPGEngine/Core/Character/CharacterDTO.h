#ifndef CHARACTERDTO_H
#define CHARACTERDTO_H

#include <string>

#include <json/json.h>

#include "CharacterModel.h"

namespace Engine {

class CharacterDTO {
public:
    CharacterDTO();
    explicit CharacterDTO( const CharacterModel& character );

    Json::Value toJson() const;

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

    int idAccount() const;
    void setIdAccount( int idAccount );

    std::string name() const;
    void setName( const std::string& name );

private:
    int _idCharacter;
    int _idAccount;
    std::string _name;
};

} // namespace Engine

#endif // CHARACTERDTO_H
