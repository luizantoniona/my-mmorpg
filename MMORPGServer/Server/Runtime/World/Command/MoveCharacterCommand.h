#ifndef MOVECHARACTERCOMMAND_H
#define MOVECHARACTERCOMMAND_H

#include <functional>
#include <string>

#include <MMORPGServer/Server/Runtime/World/Command/WorldCommand.h>

namespace Server {

class MoveCharacterCommand : public WorldCommand {
public:
    MoveCharacterCommand( int idCharacter, int dx, int dy, std::function<void( const std::string& )> respond );

    void execute( WorldRuntime& runtime ) override;

private:
    std::function<void( const std::string& )> _respond;
    int _idCharacter;
    int _dx;
    int _dy;
};

} // namespace Server

#endif // MOVECHARACTERCOMMAND_H
