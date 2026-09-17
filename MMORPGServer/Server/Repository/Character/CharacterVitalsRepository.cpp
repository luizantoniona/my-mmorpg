#include "CharacterVitalsRepository.h"

#include <MMORPGServer/Server/Database/Query.h>

namespace Server {

CharacterVitalsRepository::CharacterVitalsRepository() :
    Repository() {
}

bool CharacterVitalsRepository::create( int idCharacter, const Engine::EntityVitalsModel& vitals ) {
    const std::string sql = R"SQL(
        INSERT INTO character_vitals (
            id_character,
            health,
            max_health,
            mana,
            max_mana,
            stamina,
            max_stamina
        ) VALUES (?, ?, ?, ?, ?, ?, ?)
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );
    query.bindDouble( 2, vitals.health() );
    query.bindDouble( 3, vitals.maxHealth() );
    query.bindDouble( 4, vitals.mana() );
    query.bindDouble( 5, vitals.maxMana() );
    query.bindDouble( 6, vitals.stamina() );
    query.bindDouble( 7, vitals.maxStamina() );

    return query.exec();
}

std::unique_ptr<Engine::EntityVitalsModel> CharacterVitalsRepository::find( int idCharacter ) {
    const std::string sql = R"SQL(
        SELECT
            health,
            max_health,
            mana,
            max_mana,
            stamina,
            max_stamina
        FROM character_vitals
        WHERE id_character = ?
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );

    if ( !query.step() ) {
        return nullptr;
    }

    auto vitals = std::make_unique<Engine::EntityVitalsModel>();
    vitals->setHealth( query.getColumnDouble( 0 ) );
    vitals->setMaxHealth( query.getColumnDouble( 1 ) );
    vitals->setMana( query.getColumnDouble( 2 ) );
    vitals->setMaxMana( query.getColumnDouble( 3 ) );
    vitals->setStamina( query.getColumnDouble( 4 ) );
    vitals->setMaxStamina( query.getColumnDouble( 5 ) );

    return vitals;
}

bool CharacterVitalsRepository::save( int idCharacter, const Engine::EntityVitalsModel& vitals ) {
    const std::string sql = R"SQL(
        INSERT INTO character_vitals (
            id_character,
            health,
            max_health,
            mana,
            max_mana,
            stamina,
            max_stamina
        ) VALUES (?, ?, ?, ?, ?, ?, ?)
        ON CONFLICT (id_character) DO UPDATE SET
            health = excluded.health,
            max_health = excluded.max_health,
            mana = excluded.mana,
            max_mana = excluded.max_mana,
            stamina = excluded.stamina,
            max_stamina = excluded.max_stamina
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );
    query.bindDouble( 2, vitals.health() );
    query.bindDouble( 3, vitals.maxHealth() );
    query.bindDouble( 4, vitals.mana() );
    query.bindDouble( 5, vitals.maxMana() );
    query.bindDouble( 6, vitals.stamina() );
    query.bindDouble( 7, vitals.maxStamina() );

    return query.exec();
}

} // namespace Server
