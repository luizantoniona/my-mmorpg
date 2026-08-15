#ifndef ACCOUNTCHARACTERDTO_H
#define ACCOUNTCHARACTERDTO_H

#include <json/json.h>

#include <MMORPGEngine/Account/AccountCharacterModel.h>

namespace Engine {

class AccountCharacterDTO {
public:
    AccountCharacterDTO();
    explicit AccountCharacterDTO( const AccountCharacterModel& character );
    ~AccountCharacterDTO();

    Json::Value toJson() const;
    static AccountCharacterDTO fromJson( const Json::Value& json );

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

    std::string name() const;
    void setName( const std::string& name );

private:
    int _idCharacter;
    std::string _name;
};

} // namespace Engine

#endif // ACCOUNTCHARACTERDTO_H
