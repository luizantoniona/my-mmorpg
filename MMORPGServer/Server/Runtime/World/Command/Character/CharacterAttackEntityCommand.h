#ifndef CHARACTERATTACKENTITYCOMMAND_H
#define CHARACTERATTACKENTITYCOMMAND_H

#include <MMORPGServer/Server/Runtime/World/Command/WorldCommand.h>

namespace Server {

class CharacterAttackEntityCommand : public WorldCommand {
public:
    CharacterAttackEntityCommand( int idCharacter, int dx, int dy );

    int idCharacter() const;
    int dx() const;
    int dy() const;

private:
    int _idCharacter;
    int _dx;
    int _dy;
};

} // namespace Server

#endif // CHARACTERATTACKENTITYCOMMAND_H
