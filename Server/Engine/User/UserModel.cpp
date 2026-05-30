#include "UserModel.h"

namespace Engine {

UserModel::UserModel() :
    _idUser( 0 ),
    _dsUsername( "" ),
    _dsPassword( "" ) {
}

int UserModel::idUser() const {
    return _idUser;
}

void UserModel::setIdUser( int idUser ) {
    _idUser = idUser;
}

std::string UserModel::dsUsername() const {
    return _dsUsername;
}

void UserModel::setDsUsername( const std::string& dsUsername ) {
    _dsUsername = dsUsername;
}

std::string UserModel::dsPassword() const {
    return _dsPassword;
}

void UserModel::setDsPassword( const std::string& dsPassword ) {
    _dsPassword = dsPassword;
}

} // namespace Engine
