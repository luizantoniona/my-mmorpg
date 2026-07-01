#include "CharacterRepository.h"

#include <Engine/Database/Query.h>

namespace Engine {

CharacterRepository::CharacterRepository() :
    Repository() {
}

int CharacterRepository::createCharacter( const int idAccount, const std::string& dsName ) {
    const std::string sql = R"SQL(
        INSERT INTO character (
            id_account,
            ds_name
        ) VALUES (?, ?)
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idAccount );
    query.bindText( 2, dsName );

    if ( !query.exec() ) {
        return false;
    }

    int idCharacter = static_cast<int>( sqlite3_last_insert_rowid( _db ) );

    bool success = true;

    // TODO: Create future derivations
    // Example:
    // success &= CharacterEquipmentRepository().createEquipment( idCharacter );

    return success ? idCharacter : 0;
}

bool CharacterRepository::deleteCharacter( int idCharacter ) {
    const std::string sql = R"SQL(
        DELETE FROM character
        WHERE id_character = ?
    )SQL";

    Query query( _db, sql );
    query.bindInt( 1, idCharacter );

    return query.exec();
}

bool CharacterRepository::updateCharacter( CharacterModel character ) {
    const int idCharacter = character.idCharacter();

    bool success = true;

    // TODO: Update future derivations
    // Example:
    // success &= CharacterEquipmentRepository().updateEquipment( idCharacter, character.equipment() );

    return success;
}

std::vector<std::unique_ptr<CharacterModel>> CharacterRepository::findAllByIdAccount( const int idAccount ) {
    const std::string sql = R"SQL(
        SELECT
            id_character,
            id_account,
            ds_name
        FROM character
        WHERE id_account = ?
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idAccount );

    std::vector<std::unique_ptr<CharacterModel>> characters;

    while ( query.step() ) {
        auto character = std::make_unique<CharacterModel>();
        character->setIdCharacter( query.getColumnInt( 0 ) );
        character->setIdAccount( query.getColumnInt( 1 ) );
        character->setName( query.getColumnText( 2 ) );

        characters.push_back( std::move( character ) );
    }

    return characters;
}

std::unique_ptr<CharacterModel> CharacterRepository::findByIdAccountAndIdCharacter( const int idAccount, const int idCharacter ) {
    const std::string sql = R"SQL(
        SELECT
            id_character,
            id_account,
            ds_name
        FROM character
        WHERE id_account = ? AND id_character = ?
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idAccount );
    query.bindInt( 2, idCharacter );

    if ( query.step() ) {
        auto character = std::make_unique<CharacterModel>();
        character->setIdCharacter( query.getColumnInt( 0 ) );
        character->setIdAccount( query.getColumnInt( 1 ) );
        character->setName( query.getColumnText( 2 ) );

        // TODO: Get future derivations
        // Example:
        // auto equipment = CharacterEquipmentRepository().findByCharacterId( character->idCharacter() );
        // if ( equipment ) {
        //    character->setEquipment( *equipment );
        // }

        return character;
    }

    return nullptr;
}

} // namespace Engine
