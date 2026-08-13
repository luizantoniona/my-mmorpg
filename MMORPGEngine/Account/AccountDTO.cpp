#include "AccountDTO.h"

namespace Engine {

AccountDTO::AccountDTO() :
    _idAccount( 0 ),
    _username( "" ),
    _sessionId( "" ) {
}

AccountDTO::AccountDTO( const AccountModel& account, const std::string& sessionId ) :
    _idAccount( account.idAccount() ),
    _username( account.dsUsername() ),
    _sessionId( sessionId ) {
}

AccountDTO::~AccountDTO() = default;

Json::Value AccountDTO::toJson() const {
    Json::Value json;
    json[ "idAccount" ] = _idAccount;
    json[ "username" ] = _username;
    json[ "sessionId" ] = _sessionId;
    return json;
}

AccountDTO AccountDTO::fromJson( const Json::Value& json ) {
    AccountDTO accountDTO;

    if ( json.isMember( "idAccount" ) && json[ "idAccount" ].isInt() ) {
        accountDTO.setIdAccount( json[ "idAccount" ].asInt() );
    }

    if ( json.isMember( "username" ) && json[ "username" ].isString() ) {
        accountDTO.setUsername( json[ "username" ].asString() );
    }

    if ( json.isMember( "sessionId" ) && json[ "sessionId" ].isString() ) {
        accountDTO.setSessionId( json[ "sessionId" ].asString() );
    }

    return accountDTO;
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

std::string AccountDTO::sessionId() const {
    return _sessionId;
}

void AccountDTO::setSessionId( const std::string& sessionId ) {
    _sessionId = sessionId;
}

} // namespace Engine
