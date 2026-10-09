#ifndef UNEQUIPCHARACTERCOMMAND_H
#define UNEQUIPCHARACTERCOMMAND_H

#include <MMORPGEngine/Entity/Character/EquipmentSlotEnum.h>
#include <MMORPGServer/Server/Runtime/World/Command/WorldCommand.h>

namespace Server {

class UnequipCharacterCommand : public WorldCommand {
public:
    UnequipCharacterCommand( int idCharacter, Engine::EquipmentSlotEnum slot );

    void execute( WorldRuntime& runtime ) override;

private:
    Engine::EquipmentSlotEnum _slot;
    int _idCharacter;
};

} // namespace Server

#endif // UNEQUIPCHARACTERCOMMAND_H
