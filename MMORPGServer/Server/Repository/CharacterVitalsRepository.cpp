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

std::unique_ptr<Engine::CharacterVitalsModel> CharacterVitalsRepository::find( int idCharacter ) {
    const std::string sql = R"SQL(
        SELECT
            id_character,
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

    Engine::EntityVitalsModel vitals;
    vitals.setHealth( query.getColumnDouble( 1 ) );
    vitals.setMaxHealth( query.getColumnDouble( 2 ) );
    vitals.setMana( query.getColumnDouble( 3 ) );
    vitals.setMaxMana( query.getColumnDouble( 4 ) );
    vitals.setStamina( query.getColumnDouble( 5 ) );
    vitals.setMaxStamina( query.getColumnDouble( 6 ) );

    auto characterVitals = std::make_unique<Engine::CharacterVitalsModel>();
    characterVitals->setIdCharacter( query.getColumnInt( 0 ) );
    characterVitals->setVitals( vitals );

    return characterVitals;
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
