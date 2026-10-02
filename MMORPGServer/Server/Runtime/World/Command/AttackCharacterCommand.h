#ifndef ATTACKCHARACTERCOMMAND_H
#define ATTACKCHARACTERCOMMAND_H

#include <MMORPGServer/Server/Runtime/World/Command/WorldCommand.h>

namespace Server {

class AttackCharacterCommand : public WorldCommand {
public:
    AttackCharacterCommand( int idCharacter, int dx, int dy );

    void execute( WorldRuntime& runtime ) override;

private:
    int _idCharacter;
    int _dx;
    int _dy;
};

} // namespace Server

#endif // ATTACKCHARACTERCOMMAND_H
