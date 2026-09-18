#include "AccountDTO.h"

namespace Engine {

AccountDTO::AccountDTO() :
    _idAccount( 0 ),
    _username( "" ),
    _sessionId( "" ),
    _characters() {
}

AccountDTO::~AccountDTO() = default;

AccountDTO AccountDTO::fromModel( const AccountModel* account, const std::string& sessionId ) {
    AccountDTO dto;

    dto._idAccount = account->idAccount();
    dto._username = account->dsUsername();
    dto._sessionId = sessionId;

    dto._characters.reserve( account->characters().size() );

    for ( const auto& character : account->characters() ) {
        dto._characters.push_back( AccountCharacterDTO::fromModel( &character ) );
    }

    return dto;
}

AccountDTO AccountDTO::fromJson( const Json::Value& json ) {
    AccountDTO accountDTO;

    if ( json.isMember( "idAccount" ) && json[ "idAccount" ].isInt() ) {
        accountDTO._idAccount = json[ "idAccount" ].asInt();
    }

    if ( json.isMember( "username" ) && json[ "username" ].isString() ) {
        accountDTO._username = json[ "username" ].asString();
    }

    if ( json.isMember( "sessionId" ) && json[ "sessionId" ].isString() ) {
        accountDTO._sessionId = json[ "sessionId" ].asString();
    }

    if ( json.isMember( "characters" ) && json[ "characters" ].isArray() ) {
        for ( const auto& characterJson : json[ "characters" ] ) {
            accountDTO._characters.push_back( AccountCharacterDTO::fromJson( characterJson ) );
        }
    }

    return accountDTO;
}

Json::Value AccountDTO::toJson() const {
    Json::Value json;

    json[ "idAccount" ] = _idAccount;
    json[ "username" ] = _username;
    json[ "sessionId" ] = _sessionId;

    Json::Value charactersJson( Json::arrayValue );

    for ( const auto& character : _characters ) {
        charactersJson.append( character.toJson() );
    }

    json[ "characters" ] = charactersJson;

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

std::string AccountDTO::sessionId() const {
    return _sessionId;
}

void AccountDTO::setSessionId( const std::string& sessionId ) {
    _sessionId = sessionId;
}

const std::vector<AccountCharacterDTO>& AccountDTO::characters() const {
    return _characters;
}

void AccountDTO::setCharacters( const std::vector<AccountCharacterDTO>& characters ) {
    _characters = characters;
}

} // namespace Engine
