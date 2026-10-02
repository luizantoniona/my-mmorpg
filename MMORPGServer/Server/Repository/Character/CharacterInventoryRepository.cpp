#include "CharacterInventoryRepository.h"

#include <MMORPGServer/Server/Database/Query.h>

namespace Server {

CharacterInventoryRepository::CharacterInventoryRepository() :
    Repository() {
}

bool CharacterInventoryRepository::create( int idCharacter, const Engine::CharacterInventoryModel& inventory ) {
    const std::string sql = R"SQL(
        INSERT INTO character_inventory (
            id_character,
            position,
            id_item,
            amount
        ) VALUES (?, ?, ?, ?)
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );
    query.bindInt( 2, static_cast<int>( inventory.position() ) );
    query.bindInt( 3, static_cast<int>( inventory.idItem() ) );
    query.bindInt( 4, static_cast<int>( inventory.amount() ) );

    return query.exec();
}

std::unique_ptr<Engine::CharacterInventoryModel> CharacterInventoryRepository::find( int idCharacter, uint32_t position ) {
    const std::string sql = R"SQL(
        SELECT
            position,
            id_item,
            amount
        FROM character_inventory
        WHERE id_character = ? AND position = ?
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );
    query.bindInt( 2, static_cast<int>( position ) );

    if ( !query.step() ) {
        return nullptr;
    }

    auto inventory = std::make_unique<Engine::CharacterInventoryModel>();
    inventory->setPosition( static_cast<uint32_t>( query.getColumnInt( 0 ) ) );
    inventory->setIdItem( static_cast<uint32_t>( query.getColumnInt( 1 ) ) );
    inventory->setAmount( static_cast<uint32_t>( query.getColumnInt( 2 ) ) );

    return inventory;
}

std::vector<Engine::CharacterInventoryModel> CharacterInventoryRepository::findAll( int idCharacter ) {
    const std::string sql = R"SQL(
        SELECT
            position,
            id_item,
            amount
        FROM character_inventory
        WHERE id_character = ?
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );

    std::vector<Engine::CharacterInventoryModel> inventory;

    while ( query.step() ) {
        Engine::CharacterInventoryModel entry;
        entry.setPosition( static_cast<uint32_t>( query.getColumnInt( 0 ) ) );
        entry.setIdItem( static_cast<uint32_t>( query.getColumnInt( 1 ) ) );
        entry.setAmount( static_cast<uint32_t>( query.getColumnInt( 2 ) ) );

        inventory.push_back( std::move( entry ) );
    }

    return inventory;
}

bool CharacterInventoryRepository::save( int idCharacter, const Engine::CharacterInventoryModel& inventory ) {
    const std::string sql = R"SQL(
        INSERT INTO character_inventory (
            id_character,
            position,
            id_item,
            amount
        ) VALUES (?, ?, ?, ?)
        ON CONFLICT (id_character, position) DO UPDATE SET
            id_item = excluded.id_item,
            amount = excluded.amount
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );
    query.bindInt( 2, static_cast<int>( inventory.position() ) );
    query.bindInt( 3, static_cast<int>( inventory.idItem() ) );
    query.bindInt( 4, static_cast<int>( inventory.amount() ) );

    return query.exec();
}

} // namespace Server
