#ifndef CHARACTERMODEL_H
#define CHARACTERMODEL_H

#include <string>
#include <vector>

#include <MMORPGEngine/Entity/Character/CharacterEquipmentModel.h>
#include <MMORPGEngine/Entity/Character/CharacterInventoryModel.h>
#include <MMORPGEngine/Entity/EntityModel.h>

namespace Engine {

class CharacterModel : public EntityModel {
public:
    CharacterModel();

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

    int idAccount() const;
    void setIdAccount( int idAccount );

    std::string name() const;
    void setName( const std::string& name );

    std::vector<CharacterEquipmentModel> equipment() const;
    void setEquipment( const std::vector<CharacterEquipmentModel>& equipment );

    std::vector<CharacterInventoryModel> inventory() const;
    void setInventory( const std::vector<CharacterInventoryModel>& inventory );

private:
    std::vector<CharacterEquipmentModel> _equipment;
    std::vector<CharacterInventoryModel> _inventory;
    std::string _name;
    int _idCharacter;
    int _idAccount;
};

} // namespace Engine

#endif // CHARACTERMODEL_H
