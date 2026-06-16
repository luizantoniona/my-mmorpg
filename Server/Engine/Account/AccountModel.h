#ifndef ACCOUNTMODEL_H
#define ACCOUNTMODEL_H

#include <string>

namespace Engine {

class AccountModel {
public:
    AccountModel();

    int idAccount() const;
    void setIdAccount( int idAccount );

    std::string dsUsername() const;
    void setDsUsername( const std::string& dsUsername );

    std::string dsPassword() const;
    void setDsPassword( const std::string& dsPassword );

private:
    int _idAccount;
    std::string _dsUsername;
    std::string _dsPassword;
};

} // namespace Engine

#endif // ACCOUNTMODEL_H
