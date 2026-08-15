#include "AccountRepository.h"

#include <MMORPGServer/Server/Database/Query.h>

namespace Server {

AccountRepository::AccountRepository() :
    Repository() {
}

bool AccountRepository::createAccount( std::string& username, std::string& password ) {
    const std::string sql = R"SQL(
        INSERT INTO account (
            ds_username,
            ds_password
        ) VALUES (?, ?)
    )SQL";
    Query query( _db, sql );

    query.bindText( 1, username );
    query.bindText( 2, password );
    return query.exec();
}

std::unique_ptr<Engine::AccountModel> AccountRepository::findByUsername( const std::string& username ) {
    const std::string sql = R"SQL(
        SELECT
            id_account,
            ds_username,
            ds_password
        FROM account
        WHERE ds_username = ?
    )SQL";
    Query query( _db, sql );

    query.bindText( 1, username );

    if ( query.step() ) {
        auto account = std::make_unique<Engine::AccountModel>();
        account->setIdAccount( query.getColumnInt( 0 ) );
        account->setDsUsername( query.getColumnText( 1 ) );
        account->setDsPassword( query.getColumnText( 2 ) );
        return account;
    }

    return nullptr;
}

std::unique_ptr<Engine::AccountModel> AccountRepository::findByUsernameAndPassword( const std::string& username, const std::string& password ) {
    const std::string sql = R"SQL(
        SELECT
            id_account,
            ds_username,
            ds_password
        FROM account
        WHERE ds_username = ? AND ds_password = ?
    )SQL";
    Query query( _db, sql );

    query.bindText( 1, username );
    query.bindText( 2, password );

    if ( query.step() ) {
        auto account = std::make_unique<Engine::AccountModel>();
        account->setIdAccount( query.getColumnInt( 0 ) );
        account->setDsUsername( query.getColumnText( 1 ) );
        account->setDsPassword( query.getColumnText( 2 ) );
        return account;
    }

    return nullptr;
}

std::vector<Engine::AccountCharacterModel> AccountRepository::findCharacters( const int idAccount ) {
    const std::string sql = R"SQL(
        SELECT
            id_character,
            ds_name
        FROM character
        WHERE id_account = ?
    )SQL";

    Query query( _db, sql );

    query.bindInt( 1, idAccount );

    std::vector<Engine::AccountCharacterModel> characters;

    while ( query.step() ) {
        Engine::AccountCharacterModel character;
        character.setIdCharacter( query.getColumnInt( 0 ) );
        character.setName( query.getColumnText( 1 ) );
        character.setIdCharacter( idAccount );

        characters.push_back( character );
    }

    return characters;
}

} // namespace Server
