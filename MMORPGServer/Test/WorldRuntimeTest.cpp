#include <gtest/gtest.h>

#include <memory>
#include <set>
#include <utility>

#include <MMORPGEngine/Data/Creature/CreatureSpawnAreaModel.h>
#include <MMORPGEngine/Data/Creature/CreatureSpawnEntryModel.h>
#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/Entity/Creature/CreatureModel.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace {

std::unique_ptr<Engine::CharacterModel> makeCharacter( int idCharacter, int x, int y, int z ) {
    auto character = std::make_unique<Engine::CharacterModel>();
    character->setIdCharacter( idCharacter );
    character->position().setX( x );
    character->position().setY( y );
    character->position().setZ( z );

    return character;
}

std::unique_ptr<Engine::CreatureModel> makeCreature( int idCreature, int x, int y, int z ) {
    auto creature = std::make_unique<Engine::CreatureModel>();
    creature->setIdCreature( idCreature );
    creature->position().setX( x );
    creature->position().setY( y );
    creature->position().setZ( z );

    return creature;
}

std::unique_ptr<Engine::WorldModel> makeWorldWithSpawnArea( int z, int x, int y, uint32_t width, uint32_t height, uint32_t type, uint32_t quantity ) {
    Engine::CreatureSpawnEntryModel entry;
    entry.setType( type );
    entry.setQuantity( quantity );

    Engine::CreatureSpawnAreaModel area;
    area.setX( x );
    area.setY( y );
    area.setWidth( width );
    area.setHeight( height );
    area.setCreatures( { entry } );

    auto world = std::make_unique<Engine::WorldModel>();
    world->addFloor( z );
    world->addSpawnArea( z, area );

    return world;
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

TEST( WorldRuntimeTest, IsCharacterMoveDue_JustAdded_ReturnsTrue ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    EXPECT_TRUE( worldRuntime.isCharacterMoveDue( 1 ) );
}

TEST( WorldRuntimeTest, IsCharacterMoveDue_UnknownId_ReturnsFalse ) {
    Server::WorldRuntime worldRuntime( nullptr );

    EXPECT_FALSE( worldRuntime.isCharacterMoveDue( 1 ) );
}

TEST( WorldRuntimeTest, IsCharacterMoveDue_RightAfterMove_ReturnsFalse ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    worldRuntime.moveCharacter( 1, 1, 0, 0 );

    EXPECT_FALSE( worldRuntime.isCharacterMoveDue( 1 ) );
}

TEST( WorldRuntimeTest, IsCharacterMoveDue_AfterEnoughTicks_ReturnsTrueAgain ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    worldRuntime.moveCharacter( 1, 1, 0, 0 );
    ASSERT_FALSE( worldRuntime.isCharacterMoveDue( 1 ) );

    for ( int tick = 0; tick < 20; ++tick ) {
        worldRuntime.tick();
    }

    EXPECT_TRUE( worldRuntime.isCharacterMoveDue( 1 ) );
}

TEST( WorldRuntimeTest, IsCharacterAttackDue_JustAdded_ReturnsTrue ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    EXPECT_TRUE( worldRuntime.isCharacterAttackDue( 1 ) );
}

TEST( WorldRuntimeTest, IsCharacterAttackDue_UnknownId_ReturnsFalse ) {
    Server::WorldRuntime worldRuntime( nullptr );

    EXPECT_FALSE( worldRuntime.isCharacterAttackDue( 1 ) );
}

TEST( WorldRuntimeTest, IsCharacterAttackDue_RightAfterAttack_ReturnsFalse ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 100.0 );

    worldRuntime.attackCreature( 1, 2, 5.0, 10.0 );

    EXPECT_FALSE( worldRuntime.isCharacterAttackDue( 1 ) );
}

TEST( WorldRuntimeTest, IsCharacterAttackDue_AfterEnoughTicks_ReturnsTrueAgain ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 100.0 );

    worldRuntime.attackCreature( 1, 2, 5.0, 10.0 );
    ASSERT_FALSE( worldRuntime.isCharacterAttackDue( 1 ) );

    for ( int tick = 0; tick < 20; ++tick ) {
        worldRuntime.tick();
    }

    EXPECT_TRUE( worldRuntime.isCharacterAttackDue( 1 ) );
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

TEST( WorldRuntimeTest, IsPositionOccupied_CharacterThere_ReturnsTrue ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 5, 5, 0 ) );

    EXPECT_TRUE( worldRuntime.isPositionOccupied( 5, 5, 0 ) );
    EXPECT_FALSE( worldRuntime.isPositionOccupied( 6, 5, 0 ) );
}

TEST( WorldRuntimeTest, IsPositionOccupied_CreatureThere_ReturnsTrue ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCreature( makeCreature( 1, 5, 5, 0 ) );

    EXPECT_TRUE( worldRuntime.isPositionOccupied( 5, 5, 0 ) );
}

TEST( WorldRuntimeTest, AddCreature_ThenGet_ReturnsSameCreature ) {
    Server::WorldRuntime worldRuntime( nullptr );

    worldRuntime.addCreature( makeCreature( 1, 5, 5, 0 ) );

    const std::vector<Engine::CreatureModel> creatures = worldRuntime.creatures();

    ASSERT_EQ( creatures.size(), 1u );
    EXPECT_EQ( creatures[ 0 ].idCreature(), 1 );
    EXPECT_EQ( creatures[ 0 ].position().x(), 5 );
    EXPECT_EQ( creatures[ 0 ].position().y(), 5 );
}

TEST( WorldRuntimeTest, Creatures_Empty_ReturnsEmptyVector ) {
    Server::WorldRuntime worldRuntime( nullptr );

    EXPECT_TRUE( worldRuntime.creatures().empty() );
}

TEST( WorldRuntimeTest, Creature_ById_ReturnsSameCreature ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCreature( makeCreature( 1, 5, 5, 0 ) );

    ASSERT_NE( worldRuntime.creature( 1 ), nullptr );
    EXPECT_EQ( worldRuntime.creature( 1 )->idCreature(), 1 );
}

TEST( WorldRuntimeTest, Creature_UnknownId_ReturnsNullptr ) {
    Server::WorldRuntime worldRuntime( nullptr );

    EXPECT_EQ( worldRuntime.creature( 99 ), nullptr );
}

TEST( WorldRuntimeTest, CreatureAt_PositionMatches_ReturnsCreature ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCreature( makeCreature( 1, 5, 5, 0 ) );

    Engine::CreatureModel* creature = worldRuntime.creatureAt( 5, 5, 0 );

    ASSERT_NE( creature, nullptr );
    EXPECT_EQ( creature->idCreature(), 1 );
}

TEST( WorldRuntimeTest, CreatureAt_NoMatch_ReturnsNullptr ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCreature( makeCreature( 1, 5, 5, 0 ) );

    EXPECT_EQ( worldRuntime.creatureAt( 6, 5, 0 ), nullptr );
}

TEST( WorldRuntimeTest, AddCreature_MultipleCreatures_AllReturned ) {
    Server::WorldRuntime worldRuntime( nullptr );

    worldRuntime.addCreature( makeCreature( 1, 0, 0, 0 ) );
    worldRuntime.addCreature( makeCreature( 2, 1, 1, 0 ) );

    EXPECT_EQ( worldRuntime.creatures().size(), 2u );
}

TEST( WorldRuntimeTest, SpawnCreaturesFromAreas_SpawnsQuantityWithinArea ) {
    Server::WorldRuntime worldRuntime( makeWorldWithSpawnArea( 0, 10, 10, 5, 5, 3, 4 ) );

    worldRuntime.spawnCreaturesFromAreas();

    const std::vector<Engine::CreatureModel> creatures = worldRuntime.creatures();

    ASSERT_EQ( creatures.size(), 4u );
    for ( const Engine::CreatureModel& creature : creatures ) {
        EXPECT_EQ( creature.type(), 3u );
        EXPECT_GE( creature.position().x(), 10 );
        EXPECT_LT( creature.position().x(), 15 );
        EXPECT_GE( creature.position().y(), 10 );
        EXPECT_LT( creature.position().y(), 15 );
        EXPECT_EQ( creature.position().z(), 0 );
    }
}

TEST( WorldRuntimeTest, SpawnCreaturesFromAreas_AssignsUniqueIds ) {
    Server::WorldRuntime worldRuntime( makeWorldWithSpawnArea( 0, 0, 0, 10, 10, 1, 5 ) );

    worldRuntime.spawnCreaturesFromAreas();

    std::set<int> ids;
    for ( const Engine::CreatureModel& creature : worldRuntime.creatures() ) {
        ids.insert( creature.idCreature() );
    }

    EXPECT_EQ( ids.size(), 5u );
}

TEST( WorldRuntimeTest, SpawnCreaturesFromAreas_NullWorld_DoesNothing ) {
    Server::WorldRuntime worldRuntime( nullptr );

    worldRuntime.spawnCreaturesFromAreas();

    EXPECT_TRUE( worldRuntime.creatures().empty() );
}

TEST( WorldRuntimeTest, Tick_CreatureWalksA2x2SquareAroundSpawn_WhenCharacterIsNear ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    worldRuntime.addCreature( makeCreature( 1, 5, 5, 0 ) );

    std::vector<std::pair<int, int>> visited;
    visited.emplace_back( 5, 5 );

    for ( int step = 0; step < 4; ++step ) {
        for ( int tick = 0; tick < 20; ++tick ) {
            worldRuntime.tick();
        }

        const Engine::EntityPositionModel position = worldRuntime.creatures()[ 0 ].position();
        visited.emplace_back( position.x(), position.y() );
    }

    const std::vector<std::pair<int, int>> expected = { { 5, 5 }, { 6, 5 }, { 6, 6 }, { 5, 6 }, { 5, 5 } };
    EXPECT_EQ( visited, expected );
}

TEST( WorldRuntimeTest, Tick_CreatureDoesNotMove_WhenNoCharacterIsNear ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCreature( makeCreature( 1, 5, 5, 0 ) );

    for ( int tick = 0; tick < 100; ++tick ) {
        worldRuntime.tick();
    }

    const Engine::EntityPositionModel position = worldRuntime.creatures()[ 0 ].position();
    EXPECT_EQ( position.x(), 5 );
    EXPECT_EQ( position.y(), 5 );
}

TEST( WorldRuntimeTest, Tick_CreatureDoesNotMove_WhenCharacterIsTwoChunksAway ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 5, 5, 0 ) );
    worldRuntime.addCreature( makeCreature( 1, 500, 500, 0 ) );

    for ( int tick = 0; tick < 100; ++tick ) {
        worldRuntime.tick();
    }

    const Engine::EntityPositionModel position = worldRuntime.creatures()[ 0 ].position();
    EXPECT_EQ( position.x(), 500 );
    EXPECT_EQ( position.y(), 500 );
}

TEST( WorldRuntimeTest, Tick_CreatureDoesNotStepIntoTileOccupiedByCharacter ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 6, 5, 0 ) );
    worldRuntime.addCreature( makeCreature( 1, 5, 5, 0 ) );

    for ( int tick = 0; tick < 20; ++tick ) {
        worldRuntime.tick();
    }

    Engine::EntityPositionModel position = worldRuntime.creatures()[ 0 ].position();
    EXPECT_EQ( position.x(), 5 );
    EXPECT_EQ( position.y(), 5 );

    worldRuntime.moveCharacter( 1, 0, 0, 0 );
    worldRuntime.tick();

    position = worldRuntime.creatures()[ 0 ].position();
    EXPECT_EQ( position.x(), 6 );
    EXPECT_EQ( position.y(), 5 );
}

TEST( WorldRuntimeTest, AttackCreature_AppliesDamageAndConsumesStamina ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 100.0 );

    const bool applied = worldRuntime.attackCreature( 1, 2, 30.0, 10.0 );

    EXPECT_TRUE( applied );
    EXPECT_DOUBLE_EQ( character->vitals().stamina(), 90.0 );
    ASSERT_NE( worldRuntime.creature( 2 ), nullptr );
    EXPECT_DOUBLE_EQ( worldRuntime.creature( 2 )->vitals().health(), 70.0 );
}

TEST( WorldRuntimeTest, AttackCreature_KillsCreature_RemovesFromWorld ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 10.0 );

    worldRuntime.attackCreature( 1, 2, 30.0, 10.0 );

    EXPECT_EQ( worldRuntime.creature( 2 ), nullptr );
}

TEST( WorldRuntimeTest, AttackCreature_UnknownCharacter_ReturnsFalse ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );

    EXPECT_FALSE( worldRuntime.attackCreature( 99, 2, 5.0, 10.0 ) );
}

TEST( WorldRuntimeTest, AttackCreature_UnknownCreature_ReturnsFalse ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    EXPECT_FALSE( worldRuntime.attackCreature( 1, 99, 5.0, 10.0 ) );
}

TEST( WorldRuntimeTest, AttackCreature_PublishesEntityVitalsChangedForCharacter ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 100.0 );

    Json::Value receivedPayload;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::ENTITY_VITALS_CHANGED, [ &receivedPayload ]( const Server::WorldEvent& event ) {
        receivedPayload = event.payload();
    } );

    worldRuntime.attackCreature( 1, 2, 30.0, 10.0 );

    EXPECT_EQ( receivedPayload[ "idCharacter" ].asInt(), 1 );
}

TEST( WorldRuntimeTest, AttackCreature_PublishesCreatureVitalsChanged_WhenCreatureSurvives ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 100.0 );

    Json::Value receivedPayload;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CREATURE_VITALS_CHANGED, [ &receivedPayload ]( const Server::WorldEvent& event ) {
        receivedPayload = event.payload();
    } );

    worldRuntime.attackCreature( 1, 2, 30.0, 10.0 );

    EXPECT_EQ( receivedPayload[ "idCreature" ].asInt(), 2 );
}

TEST( WorldRuntimeTest, AttackCreature_PublishesCreatureLeft_WhenCreatureDies ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 10.0 );

    Json::Value receivedPayload;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CREATURE_LEFT, [ &receivedPayload ]( const Server::WorldEvent& event ) {
        receivedPayload = event.payload();
    } );

    worldRuntime.attackCreature( 1, 2, 30.0, 10.0 );

    EXPECT_EQ( receivedPayload[ "idCreature" ].asInt(), 2 );
}
