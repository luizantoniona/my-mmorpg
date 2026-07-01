#include "NetworkSession.h"

namespace Engine {

NetworkSession::NetworkSession() :
    _idAccount( 0 ),
    _username( "" ),
    _uuid( "" ),
    _createdAt() {
}

int NetworkSession::idAccount() const {
    return _idAccount;
}

void NetworkSession::setIdAccount( const int idAccount ) {
    _idAccount = idAccount;
}

std::string NetworkSession::username() const {
    return _username;
}

void NetworkSession::setUsername( const std::string& username ) {
    _username = username;
}

std::string NetworkSession::uuid() const {
    return _uuid;
}

void NetworkSession::setUuid( const std::string& uuid ) {
    _uuid = uuid;
}

std::chrono::system_clock::time_point NetworkSession::createdAt() const {
    return _createdAt;
}

void NetworkSession::setCreatedAt( std::chrono::system_clock::time_point createdAt ) {
    _createdAt = createdAt;
}

} // namespace Engine
