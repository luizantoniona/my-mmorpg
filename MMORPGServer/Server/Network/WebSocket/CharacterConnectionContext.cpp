#include "CharacterConnectionContext.h"

namespace Server {

CharacterConnectionContext::CharacterConnectionContext() :
    _sessionId(),
    _idCharacter( 0 ) {
}

CharacterConnectionContext::CharacterConnectionContext( const std::string& sessionId, int idCharacter ) :
    _sessionId( sessionId ),
    _idCharacter( idCharacter ) {
}

std::string CharacterConnectionContext::sessionId() const {
    return _sessionId;
}

void CharacterConnectionContext::setSessionId( const std::string& sessionId ) {
    _sessionId = sessionId;
}

int CharacterConnectionContext::idCharacter() const {
    return _idCharacter;
}

void CharacterConnectionContext::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

} // namespace Server
