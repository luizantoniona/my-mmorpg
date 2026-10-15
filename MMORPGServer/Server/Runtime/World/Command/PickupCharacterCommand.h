#ifndef PICKUPCHARACTERCOMMAND_H
#define PICKUPCHARACTERCOMMAND_H

#include <MMORPGServer/Server/Runtime/World/Command/WorldCommand.h>

namespace Server {

class PickupCharacterCommand : public WorldCommand {
public:
    PickupCharacterCommand( int idCharacter, int idGroundItem );

    void execute( WorldRuntime& runtime ) override;

private:
    int _idCharacter;
    int _idGroundItem;
};

} // namespace Server

#endif // PICKUPCHARACTERCOMMAND_H
