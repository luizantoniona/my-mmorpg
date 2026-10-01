#include "CharacterInventoryPanelControl.h"

#include <utility>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Entity/Character/CharacterInventoryDTO.h>
#include <MMORPGEngine/Network/WebSocket/ServerMessageReceiver.h>

CharacterInventoryPanelControl::CharacterInventoryPanelControl( QObject* parent ) :
    QObject( parent ) {

    connect( &Engine::Singleton<Engine::ServerMessageReceiver>::instance(), &Engine::ServerMessageReceiver::ownInventoryReceived, this, &CharacterInventoryPanelControl::onOwnInventoryReceived );
}

CharacterInventoryPanelControl::~CharacterInventoryPanelControl() = default;

QVariantList CharacterInventoryPanelControl::inventory() const {
    QVariantList result;

    for ( const Engine::CharacterInventoryModel& inventory : _inventory ) {
        QVariantMap entry;
        entry[ "position" ] = inventory.position();
        entry[ "idItem" ] = inventory.idItem();
        entry[ "amount" ] = inventory.amount();

        result.append( entry );
    }

    return result;
}

QString CharacterInventoryPanelControl::itemName( uint32_t idItem ) const {
    const Engine::ItemCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().itemCatalog();

    const Engine::ItemModel* item = catalog.item( idItem );
    if ( !item ) {
        return "";
    }

    return item->name();
}

void CharacterInventoryPanelControl::onOwnInventoryReceived( const Engine::OwnInventoryDTO& state ) {
    _inventory.clear();
    _inventory.reserve( state.items().size() );

    for ( const Engine::CharacterInventoryDTO& entry : state.items() ) {
        Engine::CharacterInventoryModel model;
        model.setPosition( entry.position() );
        model.setIdItem( entry.idItem() );
        model.setAmount( entry.amount() );

        _inventory.push_back( std::move( model ) );
    }

    emit inventoryChanged();
}
