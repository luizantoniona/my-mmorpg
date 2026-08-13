#include "AccountModel.h"

namespace Engine {

AccountModel::AccountModel() :
    _idAccount( 0 ),
    _dsUsername( "" ),
    _dsPassword( "" ) {
}

int AccountModel::idAccount() const {
    return _idAccount;
}

void AccountModel::setIdAccount( int idAccount ) {
    _idAccount = idAccount;
}

std::string AccountModel::dsUsername() const {
    return _dsUsername;
}

void AccountModel::setDsUsername( const std::string& dsUsername ) {
    _dsUsername = dsUsername;
}

std::string AccountModel::dsPassword() const {
    return _dsPassword;
}

void AccountModel::setDsPassword( const std::string& dsPassword ) {
    _dsPassword = dsPassword;
}

} // namespace Engine
