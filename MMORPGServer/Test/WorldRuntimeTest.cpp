#include <gtest/gtest.h>

#include <memory>

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace {

std::unique_ptr<Engine::CharacterModel> makeCharacter( int idCharacter, int x, int y, int z ) {
    auto character = std::make_unique<Engine::CharacterModel>();
    character->setIdCharacter( idCharacter );

    Engine::EntityPositionModel position;
    position.setX( x );
    position.setY( y );
    position.setZ( z );
    character->setPosition( position );

    return character;
}

} // namespace

TEST( WorldRuntimeTest, AddCharacter_ThenGet_ReturnsSameCharacter ) {
    Server::WorldRuntime worldRuntime( nullptr );

    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    ASSERT_NE( worldRuntime.character( 1 ), nullptr );
    EXPECT_EQ( worldRuntime.character( 1 )->idCharacter(), 1 );
}

TEST( WorldRuntimeTest, Character_UnknownId_ReturnsNullptr ) {
    Server::WorldRuntime worldRuntime( nullptr );

    EXPECT_EQ( worldRuntime.character( 99 ), nullptr );
}

TEST( WorldRuntimeTest, AddCharacter_PublishesEntityEnteredWithPosition ) {
    Server::WorldRuntime worldRuntime( nullptr );

    Json::Value receivedPayload;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::ENTITY_ENTERED, [ &receivedPayload ]( const Server::WorldEvent& event ) {
        receivedPayload = event.payload();
    } );

    worldRuntime.addCharacter( makeCharacter( 1, 5, 10, 0 ) );

    EXPECT_EQ( receivedPayload[ "idCharacter" ].asInt(), 1 );
    EXPECT_EQ( receivedPayload[ "x" ].asInt(), 5 );
    EXPECT_EQ( receivedPayload[ "y" ].asInt(), 10 );
    EXPECT_EQ( receivedPayload[ "z" ].asInt(), 0 );
}

TEST( WorldRuntimeTest, RemoveCharacter_ThenGet_ReturnsNullptr ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    worldRuntime.removeCharacter( 1 );

    EXPECT_EQ( worldRuntime.character( 1 ), nullptr );
}

TEST( WorldRuntimeTest, RemoveCharacter_UnknownId_DoesNotCrash ) {
    Server::WorldRuntime worldRuntime( nullptr );

    EXPECT_NO_THROW( worldRuntime.removeCharacter( 99 ) );
}

TEST( WorldRuntimeTest, RemoveCharacter_PublishesEntityLeftWithNearbyCharacters ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    worldRuntime.addCharacter( makeCharacter( 2, 1, 1, 0 ) );

    Json::Value receivedPayload;
    bool published = false;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::ENTITY_LEFT, [ &receivedPayload, &published ]( const Server::WorldEvent& event ) {
        receivedPayload = event.payload();
        published = true;
    } );

    worldRuntime.removeCharacter( 1 );

    ASSERT_TRUE( published );
    EXPECT_EQ( receivedPayload[ "idCharacter" ].asInt(), 1 );
    ASSERT_EQ( receivedPayload[ "nearby" ].size(), 1u );
    EXPECT_EQ( receivedPayload[ "nearby" ][ 0 ].asInt(), 2 );
}

TEST( WorldRuntimeTest, RemoveCharacter_NoNearbyCharacters_DoesNotPublish ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    bool published = false;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::ENTITY_LEFT, [ &published ]( const Server::WorldEvent& ) {
        published = true;
    } );

    worldRuntime.removeCharacter( 1 );

    EXPECT_FALSE( published );
}

TEST( WorldRuntimeTest, MoveCharacter_UpdatesPosition ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    worldRuntime.moveCharacter( 1, 3, 4, 0 );

    const Engine::EntityPositionModel position = worldRuntime.character( 1 )->position();
    EXPECT_EQ( position.x(), 3 );
    EXPECT_EQ( position.y(), 4 );
    EXPECT_EQ( position.z(), 0 );
}

TEST( WorldRuntimeTest, MoveCharacter_PublishesEntityMoved ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    Json::Value receivedPayload;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::ENTITY_MOVED, [ &receivedPayload ]( const Server::WorldEvent& event ) {
        receivedPayload = event.payload();
    } );

    worldRuntime.moveCharacter( 1, 3, 4, 0 );

    EXPECT_EQ( receivedPayload[ "idCharacter" ].asInt(), 1 );
    EXPECT_EQ( receivedPayload[ "x" ].asInt(), 3 );
    EXPECT_EQ( receivedPayload[ "y" ].asInt(), 4 );
}

TEST( WorldRuntimeTest, MoveCharacter_UnknownId_DoesNotPublish ) {
    Server::WorldRuntime worldRuntime( nullptr );

    bool published = false;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::ENTITY_MOVED, [ &published ]( const Server::WorldEvent& ) {
        published = true;
    } );

    worldRuntime.moveCharacter( 99, 3, 4, 0 );

    EXPECT_FALSE( published );
}

TEST( WorldRuntimeTest, CharactersNear_ExcludesSelf ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    EXPECT_TRUE( worldRuntime.charactersNear( 1 ).empty() );
}

TEST( WorldRuntimeTest, CharactersNear_SameChunk_IsIncluded ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    worldRuntime.addCharacter( makeCharacter( 2, 1, 1, 0 ) );

    const std::vector<int> nearby = worldRuntime.charactersNear( 1 );

    ASSERT_EQ( nearby.size(), 1u );
    EXPECT_EQ( nearby[ 0 ], 2 );
}

TEST( WorldRuntimeTest, CharactersNear_AdjacentChunk_IsIncluded ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    worldRuntime.addCharacter( makeCharacter( 2, 32, 0, 0 ) );

    const std::vector<int> nearby = worldRuntime.charactersNear( 1 );

    ASSERT_EQ( nearby.size(), 1u );
    EXPECT_EQ( nearby[ 0 ], 2 );
}

TEST( WorldRuntimeTest, CharactersNear_TwoChunksAway_IsExcluded ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    worldRuntime.addCharacter( makeCharacter( 2, 64, 0, 0 ) );

    EXPECT_TRUE( worldRuntime.charactersNear( 1 ).empty() );
}

TEST( WorldRuntimeTest, CharactersNear_DifferentFloor_IsExcluded ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    worldRuntime.addCharacter( makeCharacter( 2, 0, 0, 1 ) );

    EXPECT_TRUE( worldRuntime.charactersNear( 1 ).empty() );
}

TEST( WorldRuntimeTest, MoveCharacter_OutOfNeighborhood_UpdatesCharactersNear ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    worldRuntime.addCharacter( makeCharacter( 2, 0, 0, 0 ) );

    ASSERT_EQ( worldRuntime.charactersNear( 1 ).size(), 1u );

    worldRuntime.moveCharacter( 2, 100, 100, 0 );

    EXPECT_TRUE( worldRuntime.charactersNear( 1 ).empty() );
}
