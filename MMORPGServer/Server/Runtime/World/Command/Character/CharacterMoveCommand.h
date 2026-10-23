#ifndef CHARACTERMOVECOMMAND_H
#define CHARACTERMOVECOMMAND_H

#include <MMORPGServer/Server/Runtime/World/Command/WorldCommand.h>

namespace Server {

class CharacterMoveCommand : public WorldCommand {
public:
    CharacterMoveCommand( int idCharacter, int dx, int dy );

    int idCharacter() const;
    int dx() const;
    int dy() const;

private:
    int _idCharacter;
    int _dx;
    int _dy;
};

} // namespace Server

#endif // CHARACTERMOVECOMMAND_H
