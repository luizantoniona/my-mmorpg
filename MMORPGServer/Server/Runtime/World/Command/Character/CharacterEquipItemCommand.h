#ifndef CHARACTEREQUIPITEMCOMMAND_H
#define CHARACTEREQUIPITEMCOMMAND_H

#include <cstdint>

#include <MMORPGServer/Server/Runtime/World/Command/WorldCommand.h>

namespace Server {

class CharacterEquipItemCommand : public WorldCommand {
public:
    CharacterEquipItemCommand( int idCharacter, uint32_t inventoryPosition );

    int idCharacter() const;
    uint32_t inventoryPosition() const;

private:
    int _idCharacter;
    uint32_t _inventoryPosition;
};

} // namespace Server

#endif // CHARACTEREQUIPITEMCOMMAND_H
