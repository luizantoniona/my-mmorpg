#ifndef CHARACTERDROPITEMCOMMAND_H
#define CHARACTERDROPITEMCOMMAND_H

#include <cstdint>

#include <MMORPGServer/Server/Runtime/World/Command/WorldCommand.h>

namespace Server {

class CharacterDropItemCommand : public WorldCommand {
public:
    CharacterDropItemCommand( int idCharacter, uint32_t inventoryPosition );

    int idCharacter() const;
    uint32_t inventoryPosition() const;

private:
    int _idCharacter;
    uint32_t _inventoryPosition;
};

} // namespace Server

#endif // CHARACTERDROPITEMCOMMAND_H
