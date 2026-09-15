#include "CharacterPositionRepository.h"

#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGServer/Server/Database/Query.h>

namespace Server {

CharacterPositionRepository::CharacterPositionRepository() :
    Repository() {
}

std::unique_ptr<Engine::CharacterPositionModel> CharacterPositionRepository::find( int idCharacter ) {
    const std::string sql = R"SQL(
        SELECT
            id_character,
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

    Engine::EntityPositionModel position;
    position.setX( query.getColumnInt( 1 ) );
    position.setY( query.getColumnInt( 2 ) );
    position.setZ( query.getColumnInt( 3 ) );

    auto characterPosition = std::make_unique<Engine::CharacterPositionModel>();
    characterPosition->setIdCharacter( query.getColumnInt( 0 ) );
    characterPosition->setPosition( position );

    return characterPosition;
}

bool CharacterPositionRepository::save( const Engine::CharacterPositionModel& position ) {
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

    query.bindInt( 1, position.idCharacter() );
    query.bindInt( 2, position.position().x() );
    query.bindInt( 3, position.position().y() );
    query.bindInt( 4, position.position().z() );

    return query.exec();
}

} // namespace Server
