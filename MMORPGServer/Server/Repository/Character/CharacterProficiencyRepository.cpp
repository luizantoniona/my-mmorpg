#include "CharacterProficiencyRepository.h"

#include <MMORPGServer/Server/Database/Query.h>

namespace Server {

CharacterProficiencyRepository::CharacterProficiencyRepository() :
    Repository() {
}

bool CharacterProficiencyRepository::create( int idCharacter, const Engine::CharacterProficiencyModel& proficiency ) {
    const std::string sql = R"SQL(
        INSERT INTO character_proficiency (
            id_character,
            id_item_type,
            xp,
            lvl
        ) VALUES (?, ?, ?, ?)
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );
    query.bindInt( 2, static_cast<int>( proficiency.idItemType() ) );
    query.bindDouble( 3, proficiency.xp() );
    query.bindInt( 4, static_cast<int>( proficiency.lvl() ) );

    return query.exec();
}

std::unique_ptr<Engine::CharacterProficiencyModel> CharacterProficiencyRepository::find( int idCharacter, uint32_t idItemType ) {
    const std::string sql = R"SQL(
        SELECT
            id_item_type,
            xp,
            lvl
        FROM character_proficiency
        WHERE id_character = ? AND id_item_type = ?
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );
    query.bindInt( 2, static_cast<int>( idItemType ) );

    if ( !query.step() ) {
        return nullptr;
    }

    auto proficiency = std::make_unique<Engine::CharacterProficiencyModel>();
    proficiency->setIdItemType( static_cast<uint32_t>( query.getColumnInt( 0 ) ) );
    proficiency->setXp( query.getColumnDouble( 1 ) );
    proficiency->setLvl( static_cast<uint32_t>( query.getColumnInt( 2 ) ) );

    return proficiency;
}

std::vector<Engine::CharacterProficiencyModel> CharacterProficiencyRepository::findAll( int idCharacter ) {
    const std::string sql = R"SQL(
        SELECT
            id_item_type,
            xp,
            lvl
        FROM character_proficiency
        WHERE id_character = ?
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );

    std::vector<Engine::CharacterProficiencyModel> proficiencies;

    while ( query.step() ) {
        Engine::CharacterProficiencyModel proficiency;
        proficiency.setIdItemType( static_cast<uint32_t>( query.getColumnInt( 0 ) ) );
        proficiency.setXp( query.getColumnDouble( 1 ) );
        proficiency.setLvl( static_cast<uint32_t>( query.getColumnInt( 2 ) ) );

        proficiencies.push_back( std::move( proficiency ) );
    }

    return proficiencies;
}

bool CharacterProficiencyRepository::save( int idCharacter, const Engine::CharacterProficiencyModel& proficiency ) {
    const std::string sql = R"SQL(
        INSERT INTO character_proficiency (
            id_character,
            id_item_type,
            xp,
            lvl
        ) VALUES (?, ?, ?, ?)
        ON CONFLICT (id_character, id_item_type) DO UPDATE SET
            xp = excluded.xp,
            lvl = excluded.lvl
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );
    query.bindInt( 2, static_cast<int>( proficiency.idItemType() ) );
    query.bindDouble( 3, proficiency.xp() );
    query.bindInt( 4, static_cast<int>( proficiency.lvl() ) );

    return query.exec();
}

} // namespace Server
