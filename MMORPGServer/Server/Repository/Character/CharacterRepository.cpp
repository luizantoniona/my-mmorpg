#include "CharacterRepository.h"

#include <MMORPGServer/Server/Database/Query.h>
#include <MMORPGServer/Server/Repository/Character/CharacterEquipmentRepository.h>
#include <MMORPGServer/Server/Repository/Character/CharacterInventoryRepository.h>
#include <MMORPGServer/Server/Repository/Character/CharacterPositionRepository.h>
#include <MMORPGServer/Server/Repository/Character/CharacterProficiencyRepository.h>
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

    for ( const Engine::CharacterProficiencyModel& proficiency : character.proficiencies() ) {
        success &= CharacterProficiencyRepository().save( idCharacter, proficiency );
    }

    for ( const Engine::CharacterEquipmentModel& equipment : character.equipment() ) {
        success &= CharacterEquipmentRepository().save( idCharacter, equipment );
    }

    for ( const Engine::CharacterInventoryModel& inventory : character.inventory() ) {
        success &= CharacterInventoryRepository().save( idCharacter, inventory );
    }

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
            character->position() = *position;
        }

        auto vitals = CharacterVitalsRepository().find( character->idCharacter() );
        if ( vitals ) {
            character->vitals() = *vitals;
        }

        character->setProficiencies( CharacterProficiencyRepository().findAll( character->idCharacter() ) );
        character->setEquipment( CharacterEquipmentRepository().findAll( character->idCharacter() ) );
        character->setInventory( CharacterInventoryRepository().findAll( character->idCharacter() ) );

        return character;
    }

    return nullptr;
}

} // namespace Server
