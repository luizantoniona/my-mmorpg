#include "UserRepository.h"

#include <Engine/Database/Query.h>

namespace Engine {

UserRepository::UserRepository() :
    Repository() {}

bool UserRepository::createUser( std::string& username, std::string& password ) {
    const std::string sql = R"SQL(
        INSERT INTO user (
            ds_username,
            ds_password
        ) VALUES (?, ?)
    )SQL";
    Query query( _db, sql );

    query.bindText( 1, username );
    query.bindText( 2, password );
    return query.exec();
}

std::unique_ptr<UserModel> UserRepository::findByUsername( const std::string& username ) {
    const std::string sql = R"SQL(
        SELECT
            id_user,
            ds_username,
            ds_password
        FROM user
        WHERE ds_username = ?
    )SQL";
    Query query( _db, sql );

    query.bindText( 1, username );

    if ( query.step() ) {
        auto user = std::make_unique<UserModel>();
        user->setIdUser( query.getColumnInt( 0 ) );
        user->setDsUsername( query.getColumnText( 1 ) );
        user->setDsPassword( query.getColumnText( 2 ) );
        return user;
    }

    return nullptr;
}

std::unique_ptr<UserModel> UserRepository::findByUsernameAndPassword( const std::string& username, const std::string& password ) {
    const std::string sql = R"SQL(
        SELECT
            id_user,
            ds_username,
            ds_password
        FROM user
        WHERE ds_username = ? AND ds_password = ?
    )SQL";
    Query query( _db, sql );

    query.bindText( 1, username );
    query.bindText( 2, password );

    if ( query.step() ) {
        auto user = std::make_unique<UserModel>();
        user->setIdUser( query.getColumnInt( 0 ) );
        user->setDsUsername( query.getColumnText( 1 ) );
        user->setDsPassword( query.getColumnText( 2 ) );
        return user;
    }

    return nullptr;
}

} // namespace Engine
