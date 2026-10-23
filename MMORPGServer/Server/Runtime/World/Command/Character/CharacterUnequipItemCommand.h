#ifndef CHARACTERUNEQUIPITEMCOMMAND_H
#define CHARACTERUNEQUIPITEMCOMMAND_H

#include <MMORPGEngine/Entity/Character/EquipmentSlotEnum.h>
#include <MMORPGServer/Server/Runtime/World/Command/WorldCommand.h>

namespace Server {

class CharacterUnequipItemCommand : public WorldCommand {
public:
    CharacterUnequipItemCommand( int idCharacter, Engine::EquipmentSlotEnum slot );

    int idCharacter() const;
    Engine::EquipmentSlotEnum slot() const;

private:
    Engine::EquipmentSlotEnum _slot;
    int _idCharacter;
};

} // namespace Server

#endif // CHARACTERUNEQUIPITEMCOMMAND_H
