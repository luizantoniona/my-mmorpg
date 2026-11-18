#include "CharacterAttackResolver.h"

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>

namespace {

const QString BARE_HANDS_ITEM_TYPE = "FIST";

// TODO: dano do punho fixo por ora; depois vem do catálogo (ItemType)
constexpr double BARE_HANDS_DAMAGE = 5.0;

const Engine::ItemModel* equippedWeapon( const std::vector<Engine::CharacterEquipmentModel>& equipment ) {
    const Engine::DataManager& dataManager = Engine::Singleton<Engine::DataManager>::instance();

    for ( const Engine::CharacterEquipmentModel& entry : equipment ) {
        const Engine::ItemModel* item = dataManager.itemCatalog().item( entry.idItem() );
        if ( !item ) {
            continue;
        }

        const Engine::ItemTypeModel* itemType = dataManager.itemTypeCatalog().itemType( item->idItemType() );
        if ( itemType && itemType->category() == Engine::ItemCategoryEnum::WEAPON ) {
            return item;
        }
    }

    return nullptr;
}

} // namespace

namespace Server {

Engine::AttackModel CharacterAttackResolver::attack( const std::vector<Engine::CharacterEquipmentModel>& equipment ) {
    const Engine::ItemTypeCatalog& itemTypeCatalog = Engine::Singleton<Engine::DataManager>::instance().itemTypeCatalog();

    const Engine::ItemModel* weapon = equippedWeapon( equipment );

    if ( weapon && weapon->attack().has_value() ) {
        return weapon->attack().value();
    }

    const Engine::ItemTypeModel* itemType = itemTypeCatalog.itemType( weapon ? weapon->idItemType() : BARE_HANDS_ITEM_TYPE );

    if ( itemType && itemType->attack().has_value() ) {
        return itemType->attack().value();
    }

    return Engine::AttackModel::basic();
}

double CharacterAttackResolver::damage( const std::vector<Engine::CharacterEquipmentModel>& equipment ) {
    const Engine::ItemModel* weapon = equippedWeapon( equipment );

    if ( weapon && weapon->damage() > 0.0 ) {
        return weapon->damage();
    }

    return BARE_HANDS_DAMAGE;
}

} // namespace Server
