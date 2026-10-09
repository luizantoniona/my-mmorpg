#ifndef EQUIPCHARACTERCOMMAND_H
#define EQUIPCHARACTERCOMMAND_H

#include <cstdint>

#include <MMORPGServer/Server/Runtime/World/Command/WorldCommand.h>

namespace Server {

class EquipCharacterCommand : public WorldCommand {
public:
    EquipCharacterCommand( int idCharacter, uint32_t inventoryPosition );

    void execute( WorldRuntime& runtime ) override;

private:
    int _idCharacter;
    uint32_t _inventoryPosition;
};

} // namespace Server

#endif // EQUIPCHARACTERCOMMAND_H
