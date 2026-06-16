#include "AccountDTO.h"

namespace Engine {

AccountDTO::AccountDTO() :
    _idAccount( 0 ),
    _username( "" ) {}

AccountDTO::AccountDTO( const AccountModel& account ) :
    _idAccount( account.idAccount() ),
    _username( account.dsUsername() ) {}

Json::Value AccountDTO::toJson() const {
    Json::Value json;
    json[ "idAccount" ] = _idAccount;
    json[ "username" ] = _username;
    return json;
}

int AccountDTO::idAccount() const {
    return _idAccount;
}

void AccountDTO::setIdAccount( int idAccount ) {
    _idAccount = idAccount;
}

std::string AccountDTO::username() const {
    return _username;
}

void AccountDTO::setUsername( const std::string& username ) {
    _username = username;
}

} // namespace Engine
