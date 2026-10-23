#ifndef CHARACTERPICKUPITEMCOMMAND_H
#define CHARACTERPICKUPITEMCOMMAND_H

#include <MMORPGServer/Server/Runtime/World/Command/WorldCommand.h>

namespace Server {

class CharacterPickupItemCommand : public WorldCommand {
public:
    CharacterPickupItemCommand( int idCharacter, int idGroundItem );

    int idCharacter() const;
    int idGroundItem() const;

private:
    int _idCharacter;
    int _idGroundItem;
};

} // namespace Server

#endif // CHARACTERPICKUPITEMCOMMAND_H
