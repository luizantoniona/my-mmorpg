#ifndef CHARACTEREQUIPMENTREPOSITORY_H
#define CHARACTEREQUIPMENTREPOSITORY_H

#include <memory>
#include <vector>

#include <MMORPGEngine/Entity/Character/CharacterEquipmentModel.h>
#include <MMORPGServer/Server/Repository/Repository.h>

namespace Server {

class CharacterEquipmentRepository : public Repository {
public:
    explicit CharacterEquipmentRepository();

    bool create( int idCharacter, const Engine::CharacterEquipmentModel& equipment );

    std::unique_ptr<Engine::CharacterEquipmentModel> find( int idCharacter, Engine::EquipmentSlotEnum slot );
    std::vector<Engine::CharacterEquipmentModel> findAll( int idCharacter );

    bool save( int idCharacter, const Engine::CharacterEquipmentModel& equipment );
};

} // namespace Server

#endif // CHARACTEREQUIPMENTREPOSITORY_H
