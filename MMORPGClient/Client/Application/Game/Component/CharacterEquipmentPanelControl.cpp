#include "CharacterEquipmentPanelControl.h"

#include <utility>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Entity/Character/CharacterEquipmentDTO.h>
#include <MMORPGEngine/Entity/Character/EquipmentSlotHelper.h>
#include <MMORPGEngine/Network/WebSocket/ServerMessageReceiver.h>

CharacterEquipmentPanelControl::CharacterEquipmentPanelControl( QObject* parent ) :
    QObject( parent ) {

    connect( &Engine::Singleton<Engine::ServerMessageReceiver>::instance(), &Engine::ServerMessageReceiver::ownEquipmentReceived, this, &CharacterEquipmentPanelControl::onOwnEquipmentReceived );
}

CharacterEquipmentPanelControl::~CharacterEquipmentPanelControl() = default;

QVariantList CharacterEquipmentPanelControl::equipment() const {
    QVariantList result;

    for ( const Engine::CharacterEquipmentModel& equipment : _equipment ) {
        QVariantMap entry;
        entry[ "slot" ] = QString::fromStdString( Engine::EquipmentSlotHelper::toString( equipment.slot() ) );
        entry[ "idItem" ] = equipment.idItem();

        result.append( entry );
    }

    return result;
}

QString CharacterEquipmentPanelControl::itemName( uint32_t idItem ) const {
    const Engine::ItemCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().itemCatalog();

    const Engine::ItemModel* item = catalog.item( idItem );
    if ( !item ) {
        return "";
    }

    return item->name();
}

void CharacterEquipmentPanelControl::onOwnEquipmentReceived( const Engine::OwnEquipmentDTO& state ) {
    _equipment.clear();
    _equipment.reserve( state.equipment().size() );

    for ( const Engine::CharacterEquipmentDTO& entry : state.equipment() ) {
        Engine::CharacterEquipmentModel model;
        model.setSlot( entry.slot() );
        model.setIdItem( entry.idItem() );

        _equipment.push_back( std::move( model ) );
    }

    emit equipmentChanged();
}
