#ifndef DROPCHARACTERCOMMAND_H
#define DROPCHARACTERCOMMAND_H

#include <cstdint>

#include <MMORPGServer/Server/Runtime/World/Command/WorldCommand.h>

namespace Server {

class DropCharacterCommand : public WorldCommand {
public:
    DropCharacterCommand( int idCharacter, uint32_t inventoryPosition );

    void execute( WorldRuntime& runtime ) override;

private:
    int _idCharacter;
    uint32_t _inventoryPosition;
};

} // namespace Server

#endif // DROPCHARACTERCOMMAND_H
