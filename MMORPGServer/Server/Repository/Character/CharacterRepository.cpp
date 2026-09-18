#include "CharacterRepository.h"

#include <MMORPGServer/Server/Database/Query.h>
#include <MMORPGServer/Server/Repository/Character/CharacterPositionRepository.h>
#include <MMORPGServer/Server/Repository/Character/CharacterVitalsRepository.h>

namespace Server {

CharacterRepository::CharacterRepository() :
    Repository() {
}

int CharacterRepository::createCharacter( const int idAccount, const std::string& dsName, const Engine::EntityPositionModel& spawnPosition, const Engine::EntityVitalsModel& spawnVitals ) {
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

    success &= CharacterPositionRepository().create( idCharacter, spawnPosition );
    success &= CharacterVitalsRepository().create( idCharacter, spawnVitals );

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

bool CharacterRepository::updateCharacter( Engine::CharacterModel character ) {
    const int idCharacter = character.idCharacter();

    bool success = true;

    success &= CharacterPositionRepository().save( idCharacter, character.position() );
    success &= CharacterVitalsRepository().save( idCharacter, character.vitals() );

    // TODO: Update future derivations
    // Example:
    // success &= CharacterEquipmentRepository().updateEquipment( idCharacter, character.equipment() );

    return success;
}

std::unique_ptr<Engine::CharacterModel> CharacterRepository::findByIdAccountAndIdCharacter( const int idAccount, const int idCharacter ) {
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
        auto character = std::make_unique<Engine::CharacterModel>();
        character->setIdCharacter( query.getColumnInt( 0 ) );
        character->setIdAccount( query.getColumnInt( 1 ) );
        character->setName( query.getColumnText( 2 ) );

        auto position = CharacterPositionRepository().find( character->idCharacter() );
        if ( position ) {
            character->setPosition( *position );
        }

        auto vitals = CharacterVitalsRepository().find( character->idCharacter() );
        if ( vitals ) {
            character->setVitals( *vitals );
        }

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

} // namespace Server
