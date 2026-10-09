#include "CharacterCombatActionResolver.h"

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Data/Item/ItemCategoryHelper.h>

namespace {

const QString BARE_HANDS_ITEM_TYPE = "FIST";

} // namespace

namespace Server {

std::set<Engine::CombatActionEnum> CharacterCombatActionResolver::resolve( const std::vector<Engine::CharacterEquipmentModel>& equipment ) {
    const Engine::DataManager& dataManager = Engine::Singleton<Engine::DataManager>::instance();

    std::set<Engine::CombatActionEnum> actions = { Engine::CombatActionEnum::DODGE };
    std::set<Engine::CombatActionEnum> disabledActions;
    bool weaponEquipped = false;

    for ( const Engine::CharacterEquipmentModel& entry : equipment ) {
        const Engine::ItemModel* item = dataManager.itemCatalog().item( entry.idItem() );
        if ( !item ) {
            continue;
        }

        const Engine::ItemTypeModel* itemType = dataManager.itemTypeCatalog().itemType( item->idItemType() );
        if ( !itemType ) {
            continue;
        }

        const std::set<Engine::CombatActionEnum> granted = Engine::ItemCategoryHelper::grantedActions( itemType->category() );
        actions.insert( granted.begin(), granted.end() );
        disabledActions.insert( itemType->disabledActions().begin(), itemType->disabledActions().end() );

        weaponEquipped = weaponEquipped || itemType->category() == Engine::ItemCategoryEnum::WEAPON;
    }

    if ( !weaponEquipped ) {
        actions.insert( Engine::CombatActionEnum::ATTACK );

        const Engine::ItemTypeModel* bareHands = dataManager.itemTypeCatalog().itemType( BARE_HANDS_ITEM_TYPE );
        if ( bareHands ) {
            disabledActions.insert( bareHands->disabledActions().begin(), bareHands->disabledActions().end() );
        }
    }

    for ( const Engine::CombatActionEnum disabled : disabledActions ) {
        actions.erase( disabled );
    }

    return actions;
}

} // namespace Server
