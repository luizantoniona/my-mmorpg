#include "CharacterAppearanceRepository.h"

#include <MMORPGEngine/Entity/Character/CharacterAppearanceFeatureHelper.h>
#include <MMORPGServer/Server/Database/Query.h>

namespace Server {

CharacterAppearanceRepository::CharacterAppearanceRepository() :
    Repository() {
}

bool CharacterAppearanceRepository::create( int idCharacter, const Engine::CharacterAppearanceModel& appearance ) {
    const std::string sql = R"SQL(
        INSERT INTO character_appearance (
            id_character,
            feature,
            id_value
        ) VALUES (?, ?, ?)
    )SQL";

    bool success = true;

    for ( const auto& selection : appearance.selections() ) {
        Query query( _db, sql );

        query.bindInt( 1, idCharacter );
        query.bindText( 2, Engine::CharacterAppearanceFeatureHelper::toString( selection.first ) );
        query.bindText( 3, selection.second.toStdString() );

        success &= query.exec();
    }

    return success;
}

Engine::CharacterAppearanceModel CharacterAppearanceRepository::find( int idCharacter ) {
    const std::string sql = R"SQL(
        SELECT
            feature,
            id_value
        FROM character_appearance
        WHERE id_character = ?
    )SQL";
    Query query( _db, sql );

    query.bindInt( 1, idCharacter );

    Engine::CharacterAppearanceModel appearance;

    while ( query.step() ) {
        const Engine::CharacterAppearanceFeatureEnum feature = Engine::CharacterAppearanceFeatureHelper::fromString( query.getColumnText( 0 ) );

        if ( feature == Engine::CharacterAppearanceFeatureEnum::UNKNOWN ) {
            continue;
        }

        appearance.setId( feature, QString::fromStdString( query.getColumnText( 1 ) ) );
    }

    return appearance;
}

} // namespace Server
