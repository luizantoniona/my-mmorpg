#include <gtest/gtest.h>

#include <MMORPGEngine/Entity/Character/CharacterIntentActionDTO.h>
#include <MMORPGEngine/Entity/Character/OwnCombatDTO.h>
#include <MMORPGEngine/Entity/CombatActionHelper.h>
#include <MMORPGEngine/Network/WebSocket/ClientMessageTypeHelper.h>
#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

TEST( OwnCombatDTOTest, FromModel_CopiesAvailableActions ) {
    Engine::CharacterModel character;
    character.setIdCharacter( 7 );
    character.combat().setAvailableActions( { Engine::CombatActionEnum::ATTACK, Engine::CombatActionEnum::BLOCK } );

    const Engine::OwnCombatDTO dto = Engine::OwnCombatDTO::fromModel( &character );

    EXPECT_EQ( dto.idCharacter(), 7 );
    EXPECT_EQ( dto.actions(), character.combat().availableActions() );
}

TEST( OwnCombatDTOTest, ToJsonThenFromJson_RoundTrips ) {
    Engine::OwnCombatDTO dto;
    dto.setIdCharacter( 3 );
    dto.setActions( { Engine::CombatActionEnum::ATTACK, Engine::CombatActionEnum::DODGE } );

    const Json::Value json = dto.toJson();
    const Engine::OwnCombatDTO reloaded = Engine::OwnCombatDTO::fromJson( json );

    EXPECT_EQ( Engine::ServerMessageTypeHelper::fromMessage( json ), Engine::ServerMessageType::OWN_COMBAT );
    EXPECT_EQ( reloaded.idCharacter(), 3 );
    EXPECT_EQ( reloaded.actions(), dto.actions() );
}

TEST( OwnCombatDTOTest, FromJson_UnknownAction_IsSkipped ) {
    Json::Value json;
    json[ "actions" ].append( "ATTACK" );
    json[ "actions" ].append( "FLY" );

    const Engine::OwnCombatDTO dto = Engine::OwnCombatDTO::fromJson( json );

    EXPECT_EQ( dto.actions().size(), 1u );
}

TEST( CombatActionHelperTest, ToStringThenFromString_RoundTrips ) {
    for ( Engine::CombatActionEnum action : { Engine::CombatActionEnum::ATTACK, Engine::CombatActionEnum::DODGE, Engine::CombatActionEnum::BLOCK } ) {
        EXPECT_EQ( Engine::CombatActionHelper::fromString( Engine::CombatActionHelper::toString( action ) ), action );
    }
}

TEST( CharacterIntentActionDTOTest, ToJsonThenFromJson_RoundTripsActionAndDirection ) {
    Engine::CharacterIntentActionDTO dto;
    dto.setAction( Engine::CombatActionEnum::ATTACK );
    dto.setDx( 1 );
    dto.setDy( -1 );

    const Json::Value json = dto.toJson();
    const Engine::CharacterIntentActionDTO reloaded = Engine::CharacterIntentActionDTO::fromJson( json );

    EXPECT_EQ( Engine::ClientMessageTypeHelper::fromMessage( json ), Engine::ClientMessageType::CHARACTER_INTENT_ACTION );
    EXPECT_EQ( json[ "action" ].asString(), "ATTACK" );
    ASSERT_TRUE( reloaded.action().has_value() );
    EXPECT_EQ( *reloaded.action(), Engine::CombatActionEnum::ATTACK );
    EXPECT_EQ( reloaded.dx(), 1 );
    EXPECT_EQ( reloaded.dy(), -1 );
}

TEST( CharacterIntentActionDTOTest, FromJson_UnknownOrMissingAction_HasNoAction ) {
    Json::Value unknown;
    unknown[ "action" ] = "FLY";

    EXPECT_FALSE( Engine::CharacterIntentActionDTO::fromJson( unknown ).action().has_value() );
    EXPECT_FALSE( Engine::CharacterIntentActionDTO::fromJson( Json::Value( Json::objectValue ) ).action().has_value() );
}
