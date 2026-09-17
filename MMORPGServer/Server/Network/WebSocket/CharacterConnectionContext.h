#ifndef CHARACTERCONNECTIONCONTEXT_H
#define CHARACTERCONNECTIONCONTEXT_H

#include <string>

namespace Server {

class CharacterConnectionContext {
public:
    CharacterConnectionContext();
    CharacterConnectionContext( const std::string& sessionId, int idCharacter );

    std::string sessionId() const;
    void setSessionId( const std::string& sessionId );

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

private:
    std::string _sessionId;
    int _idCharacter;
};

} // namespace Server

#endif // CHARACTERCONNECTIONCONTEXT_H
