#ifndef ACCOUNTCHARACTERMODEL_H
#define ACCOUNTCHARACTERMODEL_H

#include <string>

namespace Engine {

class AccountCharacterModel {
public:
    AccountCharacterModel();
    ~AccountCharacterModel();

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

#endif // ACCOUNTCHARACTERMODEL_H
