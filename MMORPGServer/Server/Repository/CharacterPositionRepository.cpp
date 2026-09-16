#include "CharacterPositionRepository.h"

#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGServer/Server/Database/Query.h>

namespace Server {

CharacterPositionRepository::CharacterPositionRepository() :
    Repository() {
}

bool CharacterPositionRepository::create( int idCharacter, const Engine::EntityPositionModel& position ) {
    const std::string sql = R"SQL(
        INSERT INTO character_position (
            id_character,
            x,
            y,
            z
        ) VALUES (?, ?, ?, ?)
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );
    query.bindInt( 2, position.x() );
    query.bindInt( 3, position.y() );
    query.bindInt( 4, position.z() );

    return query.exec();
}

std::unique_ptr<Engine::EntityPositionModel> CharacterPositionRepository::find( int idCharacter ) {
    const std::string sql = R"SQL(
        SELECT
            x,
            y,
            z
        FROM character_position
        WHERE id_character = ?
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );

    if ( !query.step() ) {
        return nullptr;
    }

    auto position = std::make_unique<Engine::EntityPositionModel>();
    position->setX( query.getColumnInt( 0 ) );
    position->setY( query.getColumnInt( 1 ) );
    position->setZ( query.getColumnInt( 2 ) );

    return position;
}

bool CharacterPositionRepository::save( int idCharacter, const Engine::EntityPositionModel& position ) {
    const std::string sql = R"SQL(
        INSERT INTO character_position (
            id_character,
            x,
            y,
            z
        ) VALUES (?, ?, ?, ?)
        ON CONFLICT (id_character) DO UPDATE SET
            x = excluded.x,
            y = excluded.y,
            z = excluded.z
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );
    query.bindInt( 2, position.x() );
    query.bindInt( 3, position.y() );
    query.bindInt( 4, position.z() );

    return query.exec();
}

} // namespace Server
