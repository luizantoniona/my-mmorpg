#include "CharacterEquipmentRepository.h"

#include <MMORPGServer/Server/Database/Query.h>

namespace Server {

CharacterEquipmentRepository::CharacterEquipmentRepository() :
    Repository() {
}

bool CharacterEquipmentRepository::create( int idCharacter, const Engine::CharacterEquipmentModel& equipment ) {
    const std::string sql = R"SQL(
        INSERT INTO character_equipment (
            id_character,
            slot,
            id_item
        ) VALUES (?, ?, ?)
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );
    query.bindInt( 2, static_cast<int>( equipment.slot() ) );
    query.bindInt( 3, static_cast<int>( equipment.idItem() ) );

    return query.exec();
}

std::unique_ptr<Engine::CharacterEquipmentModel> CharacterEquipmentRepository::find( int idCharacter, Engine::EquipmentSlotEnum slot ) {
    const std::string sql = R"SQL(
        SELECT
            slot,
            id_item
        FROM character_equipment
        WHERE id_character = ? AND slot = ?
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );
    query.bindInt( 2, static_cast<int>( slot ) );

    if ( !query.step() ) {
        return nullptr;
    }

    auto equipment = std::make_unique<Engine::CharacterEquipmentModel>();
    equipment->setSlot( static_cast<Engine::EquipmentSlotEnum>( query.getColumnInt( 0 ) ) );
    equipment->setIdItem( static_cast<uint32_t>( query.getColumnInt( 1 ) ) );

    return equipment;
}

std::vector<Engine::CharacterEquipmentModel> CharacterEquipmentRepository::findAll( int idCharacter ) {
    const std::string sql = R"SQL(
        SELECT
            slot,
            id_item
        FROM character_equipment
        WHERE id_character = ?
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );

    std::vector<Engine::CharacterEquipmentModel> equipment;

    while ( query.step() ) {
        Engine::CharacterEquipmentModel entry;
        entry.setSlot( static_cast<Engine::EquipmentSlotEnum>( query.getColumnInt( 0 ) ) );
        entry.setIdItem( static_cast<uint32_t>( query.getColumnInt( 1 ) ) );

        equipment.push_back( std::move( entry ) );
    }

    return equipment;
}

bool CharacterEquipmentRepository::save( int idCharacter, const Engine::CharacterEquipmentModel& equipment ) {
    const std::string sql = R"SQL(
        INSERT INTO character_equipment (
            id_character,
            slot,
            id_item
        ) VALUES (?, ?, ?)
        ON CONFLICT (id_character, slot) DO UPDATE SET
            id_item = excluded.id_item
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );
    query.bindInt( 2, static_cast<int>( equipment.slot() ) );
    query.bindInt( 3, static_cast<int>( equipment.idItem() ) );

    return query.exec();
}

} // namespace Server
