#ifndef ACCOUNTMODEL_H
#define ACCOUNTMODEL_H

#include <string>
#include <vector>

#include <MMORPGEngine/Account/AccountCharacterModel.h>

namespace Engine {

class AccountModel {
public:
    AccountModel();
    ~AccountModel();

    int idAccount() const;
    void setIdAccount( int idAccount );

    std::string dsUsername() const;
    void setDsUsername( const std::string& dsUsername );

    std::string dsPassword() const;
    void setDsPassword( const std::string& dsPassword );

    const std::vector<AccountCharacterModel>& characters() const;
    void setCharacters( const std::vector<AccountCharacterModel>& characters );

private:
    int _idAccount;
    std::string _dsUsername;
    std::string _dsPassword;
    std::vector<AccountCharacterModel> _characters;
};

} // namespace Engine

#endif // ACCOUNTMODEL_H
