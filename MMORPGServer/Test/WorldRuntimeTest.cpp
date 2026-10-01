#include <gtest/gtest.h>

#include <memory>
#include <set>
#include <utility>

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/Creature/CreatureSpawnAreaModel.h>
#include <MMORPGEngine/Data/Creature/CreatureSpawnEntryModel.h>
#include <MMORPGEngine/Data/Creature/CreatureTypeModel.h>
#include <MMORPGEngine/Data/DataManager.h>
#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/Entity/Creature/CreatureModel.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/World/WorldConstants.h>
#include <MMORPGEngine/World/WorldModel.h>
#include <MMORPGServer/Server/Runtime/World/Command/WorldCommand.h>
#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace {

class RecordingCommand : public Server::WorldCommand {
public:
    explicit RecordingCommand( std::vector<int>& executionOrder, int id ) :
        _executionOrder( executionOrder ),
        _id( id ) {
    }

    void execute( Server::WorldRuntime& ) override {
        _executionOrder.push_back( _id );
    }

private:
    std::vector<int>& _executionOrder;
    int _id;
};

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
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CHARACTER_ENTERED, [ &receivedPayload ]( const Server::WorldEvent& event ) {
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
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CHARACTER_LEFT, [ &receivedPayload, &published ]( const Server::WorldEvent& event ) {
        receivedPayload = event.payload();
        published = true;
    } );

    worldRuntime.removeCharacter( 1 );

    ASSERT_TRUE( published );
    EXPECT_EQ( receivedPayload[ "idCharacter" ].asInt(), 1 );
    ASSERT_EQ( receivedPayload[ "nearby" ].size(), 1u );
    EXPECT_EQ( receivedPayload[ "nearby" ][ 0 ].asInt(), 2 );
}

TEST( WorldRuntimeTest, RemoveCharacter_NoNearbyCharacters_StillPublishesWithEmptyNearby ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    Json::Value receivedPayload;
    bool published = false;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CHARACTER_LEFT, [ &receivedPayload, &published ]( const Server::WorldEvent& event ) {
        receivedPayload = event.payload();
        published = true;
    } );

    worldRuntime.removeCharacter( 1 );

    ASSERT_TRUE( published );
    EXPECT_EQ( receivedPayload[ "idCharacter" ].asInt(), 1 );
    EXPECT_EQ( receivedPayload[ "nearby" ].size(), 0u );
}

TEST( WorldRuntimeTest, RemoveCharacter_UnknownId_DoesNotPublish ) {
    Server::WorldRuntime worldRuntime( nullptr );

    bool published = false;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CHARACTER_LEFT, [ &published ]( const Server::WorldEvent& ) {
        published = true;
    } );

    worldRuntime.removeCharacter( 42 );

    EXPECT_FALSE( published );
}

TEST( WorldRuntimeTest, MoveCharacter_UpdatesPosition ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    worldRuntime.movementSystem().moveCharacter( 1, 3, 4, 0 );

    const Engine::EntityPositionModel position = worldRuntime.character( 1 )->position();
    EXPECT_EQ( position.x(), 3 );
    EXPECT_EQ( position.y(), 4 );
    EXPECT_EQ( position.z(), 0 );
}

TEST( WorldRuntimeTest, MoveCharacter_PublishesEntityMoved ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    Json::Value receivedPayload;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CHARACTER_MOVED, [ &receivedPayload ]( const Server::WorldEvent& event ) {
        receivedPayload = event.payload();
    } );

    worldRuntime.movementSystem().moveCharacter( 1, 3, 4, 0 );

    EXPECT_EQ( receivedPayload[ "idCharacter" ].asInt(), 1 );
    EXPECT_EQ( receivedPayload[ "x" ].asInt(), 3 );
    EXPECT_EQ( receivedPayload[ "y" ].asInt(), 4 );
}

TEST( WorldRuntimeTest, MoveCharacter_UnknownId_DoesNotPublish ) {
    Server::WorldRuntime worldRuntime( nullptr );

    bool published = false;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CHARACTER_MOVED, [ &published ]( const Server::WorldEvent& ) {
        published = true;
    } );

    worldRuntime.movementSystem().moveCharacter( 99, 3, 4, 0 );

    EXPECT_FALSE( published );
}

TEST( WorldRuntimeTest, CharacterMovement_JustAdded_IsReady ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    EXPECT_TRUE( character->movement().isReady( worldRuntime.tickRate() ) );
}

TEST( WorldRuntimeTest, CharacterMovement_RightAfterMove_IsNotReady ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    worldRuntime.movementSystem().moveCharacter( 1, 1, 0, 0 );

    EXPECT_FALSE( character->movement().isReady( worldRuntime.tickRate() ) );
}

TEST( WorldRuntimeTest, CharacterMovement_AfterEnoughTicks_IsReadyAgain ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    worldRuntime.movementSystem().moveCharacter( 1, 1, 0, 0 );
    ASSERT_FALSE( character->movement().isReady( worldRuntime.tickRate() ) );

    for ( int tick = 0; tick < 20; ++tick ) {
        worldRuntime.tick();
    }

    EXPECT_TRUE( character->movement().isReady( worldRuntime.tickRate() ) );
}

TEST( WorldRuntimeTest, CharacterCombat_JustAdded_IsReady ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    EXPECT_TRUE( character->combat().isReady( worldRuntime.tickRate() ) );
}

TEST( WorldRuntimeTest, CharacterCombat_RightAfterAttack_IsNotReady ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 100.0 );

    worldRuntime.combatSystem().attack( 1, 1, 0 );

    EXPECT_FALSE( character->combat().isReady( worldRuntime.tickRate() ) );
}

TEST( WorldRuntimeTest, CharacterCombat_AfterEnoughTicks_IsReadyAgain ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 100.0 );

    worldRuntime.combatSystem().attack( 1, 1, 0 );
    ASSERT_FALSE( character->combat().isReady( worldRuntime.tickRate() ) );

    for ( int tick = 0; tick < 20; ++tick ) {
        worldRuntime.tick();
    }

    EXPECT_TRUE( character->combat().isReady( worldRuntime.tickRate() ) );
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

TEST( WorldRuntimeTest, CharactersNearPosition_IncludesEveryoneInRange ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    worldRuntime.addCharacter( makeCharacter( 2, 5, 5, 0 ) );

    Engine::EntityPositionModel position;
    position.setX( 1 );
    position.setY( 1 );
    position.setZ( 0 );

    EXPECT_EQ( worldRuntime.charactersNear( position ).size(), 2u );
}

TEST( WorldRuntimeTest, CharactersNearPosition_TwoChunksAway_IsExcluded ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    Engine::EntityPositionModel position;
    position.setX( 2 * Engine::WorldConstants::CHUNK_SIZE );
    position.setY( 0 );
    position.setZ( 0 );

    EXPECT_TRUE( worldRuntime.charactersNear( position ).empty() );
}

TEST( WorldRuntimeTest, CharactersNearPosition_DifferentFloor_IsExcluded ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    Engine::EntityPositionModel position;
    position.setX( 0 );
    position.setY( 0 );
    position.setZ( 1 );

    EXPECT_TRUE( worldRuntime.charactersNear( position ).empty() );
}

TEST( WorldRuntimeTest, MoveCharacter_OutOfNeighborhood_UpdatesCharactersNear ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    worldRuntime.addCharacter( makeCharacter( 2, 0, 0, 0 ) );

    ASSERT_EQ( worldRuntime.charactersNear( 1 ).size(), 1u );

    worldRuntime.movementSystem().moveCharacter( 2, 100, 100, 0 );

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

    worldRuntime.spawnSystem().spawnCreaturesFromAreas();

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

    worldRuntime.spawnSystem().spawnCreaturesFromAreas();

    std::set<int> ids;
    for ( const Engine::CreatureModel& creature : worldRuntime.creatures() ) {
        ids.insert( creature.idCreature() );
    }

    EXPECT_EQ( ids.size(), 5u );
}

TEST( WorldRuntimeTest, SpawnCreaturesFromAreas_NullWorld_DoesNothing ) {
    Server::WorldRuntime worldRuntime( nullptr );

    worldRuntime.spawnSystem().spawnCreaturesFromAreas();

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

    worldRuntime.movementSystem().moveCharacter( 1, 0, 0, 0 );
    worldRuntime.tick();

    position = worldRuntime.creatures()[ 0 ].position();
    EXPECT_EQ( position.x(), 6 );
    EXPECT_EQ( position.y(), 5 );
}

TEST( WorldRuntimeTest, AttackTile_WithCreature_AppliesDamage ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 100.0 );

    const bool applied = worldRuntime.combatSystem().resolveCharacterAttack( 1, 1, 0, 0, 30.0 );

    EXPECT_TRUE( applied );
    ASSERT_NE( worldRuntime.creature( 2 ), nullptr );
    EXPECT_DOUBLE_EQ( worldRuntime.creature( 2 )->vitals().health(), 70.0 );
}

TEST( WorldRuntimeTest, AttackTile_KillsCreature_RemovesFromWorld ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 10.0 );

    worldRuntime.combatSystem().resolveCharacterAttack( 1, 1, 0, 0, 30.0 );

    EXPECT_EQ( worldRuntime.creature( 2 ), nullptr );
}

TEST( WorldRuntimeTest, AttackTile_UnknownCharacter_ReturnsFalse ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );

    EXPECT_FALSE( worldRuntime.combatSystem().resolveCharacterAttack( 99, 1, 0, 0, 5.0 ) );
}

TEST( WorldRuntimeTest, AttackTile_EmptyTile_ResolvesWithoutHittingAnything ) {
    Server::WorldRuntime worldRuntime( nullptr );
    worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );

    EXPECT_TRUE( worldRuntime.combatSystem().resolveCharacterAttack( 1, 1, 0, 0, 30.0 ) );
}

TEST( WorldRuntimeTest, AttackTile_EmptyTile_PublishesEntityAttackedWithTargetTile ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );

    Json::Value receivedPayload;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CHARACTER_ATTACKED, [ &receivedPayload ]( const Server::WorldEvent& event ) {
        receivedPayload = event.payload();
    } );

    worldRuntime.combatSystem().resolveCharacterAttack( 1, 7, 8, 0, 30.0 );

    EXPECT_EQ( receivedPayload[ "idCharacter" ].asInt(), 1 );
    EXPECT_EQ( receivedPayload[ "x" ].asInt(), 7 );
    EXPECT_EQ( receivedPayload[ "y" ].asInt(), 8 );
    EXPECT_EQ( receivedPayload[ "z" ].asInt(), 0 );
}

TEST( WorldRuntimeTest, AttackTile_EmptyTile_DoesNotPublishCreatureEvents ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );

    bool creatureEventPublished = false;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CREATURE_VITALS_CHANGED, [ &creatureEventPublished ]( const Server::WorldEvent& ) {
        creatureEventPublished = true;
    } );
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CREATURE_LEFT, [ &creatureEventPublished ]( const Server::WorldEvent& ) {
        creatureEventPublished = true;
    } );

    worldRuntime.combatSystem().resolveCharacterAttack( 1, 1, 0, 0, 30.0 );

    EXPECT_FALSE( creatureEventPublished );
}

TEST( WorldRuntimeTest, AttackTile_CreatureOnAnotherFloor_IsNotHit ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 1 ) );
    creature->vitals().setHealth( 100.0 );

    worldRuntime.combatSystem().resolveCharacterAttack( 1, 1, 0, 0, 30.0 );

    ASSERT_NE( worldRuntime.creature( 2 ), nullptr );
    EXPECT_DOUBLE_EQ( worldRuntime.creature( 2 )->vitals().health(), 100.0 );
}

TEST( WorldRuntimeTest, Attack_ConsumesStaminaAndPublishesVitalsChanged_AtCastStart ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );

    Json::Value receivedPayload;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CHARACTER_VITALS_CHANGED, [ &receivedPayload ]( const Server::WorldEvent& event ) {
        receivedPayload = event.payload();
    } );

    EXPECT_TRUE( worldRuntime.combatSystem().attack( 1, 1, 0 ) );

    EXPECT_DOUBLE_EQ( character->vitals().stamina(), 90.0 );
    EXPECT_EQ( receivedPayload[ "idCharacter" ].asInt(), 1 );
}

TEST( WorldRuntimeTest, AttackTile_PublishesCreatureVitalsChanged_WhenCreatureSurvives ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 100.0 );

    Json::Value receivedPayload;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CREATURE_VITALS_CHANGED, [ &receivedPayload ]( const Server::WorldEvent& event ) {
        receivedPayload = event.payload();
    } );

    worldRuntime.combatSystem().resolveCharacterAttack( 1, 1, 0, 0, 30.0 );

    EXPECT_EQ( receivedPayload[ "idCreature" ].asInt(), 2 );
}

TEST( WorldRuntimeTest, EnqueueCommand_NotExecuted_BeforeTick ) {
    Server::WorldRuntime worldRuntime( nullptr );
    std::vector<int> executionOrder;

    worldRuntime.enqueueCommand( std::make_unique<RecordingCommand>( executionOrder, 1 ) );

    EXPECT_TRUE( executionOrder.empty() );
}

TEST( WorldRuntimeTest, EnqueueCommand_ExecutedOnNextTick ) {
    Server::WorldRuntime worldRuntime( nullptr );
    std::vector<int> executionOrder;

    worldRuntime.enqueueCommand( std::make_unique<RecordingCommand>( executionOrder, 1 ) );
    worldRuntime.tick();

    ASSERT_EQ( executionOrder.size(), 1u );
    EXPECT_EQ( executionOrder[ 0 ], 1 );
}

TEST( WorldRuntimeTest, EnqueueCommand_MultipleCommands_ExecutedInOrderOnce ) {
    Server::WorldRuntime worldRuntime( nullptr );
    std::vector<int> executionOrder;

    worldRuntime.enqueueCommand( std::make_unique<RecordingCommand>( executionOrder, 1 ) );
    worldRuntime.enqueueCommand( std::make_unique<RecordingCommand>( executionOrder, 2 ) );
    worldRuntime.tick();
    worldRuntime.tick();

    const std::vector<int> expected = { 1, 2 };
    EXPECT_EQ( executionOrder, expected );
}

TEST( WorldRuntimeTest, AttackTile_PublishesCreatureLeft_WhenCreatureDies ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 10.0 );

    Json::Value receivedPayload;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CREATURE_LEFT, [ &receivedPayload ]( const Server::WorldEvent& event ) {
        receivedPayload = event.payload();
    } );

    worldRuntime.combatSystem().resolveCharacterAttack( 1, 1, 0, 0, 30.0 );

    EXPECT_EQ( receivedPayload[ "idCreature" ].asInt(), 2 );
}

TEST( WorldRuntimeTest, AttackTile_WithCreature_PublishesEntityAttackedWithTargetTile ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 100.0 );

    Json::Value receivedPayload;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CHARACTER_ATTACKED, [ &receivedPayload ]( const Server::WorldEvent& event ) {
        receivedPayload = event.payload();
    } );

    worldRuntime.combatSystem().resolveCharacterAttack( 1, 1, 0, 0, 30.0 );

    EXPECT_EQ( receivedPayload[ "idCharacter" ].asInt(), 1 );
    EXPECT_EQ( receivedPayload[ "x" ].asInt(), 1 );
    EXPECT_EQ( receivedPayload[ "y" ].asInt(), 0 );
    EXPECT_EQ( receivedPayload[ "z" ].asInt(), 0 );
}

TEST( WorldRuntimeTest, AttackTile_PublishesEntityAttacked_WhenCreatureDies ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 10.0 );

    Json::Value receivedPayload;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CHARACTER_ATTACKED, [ &receivedPayload ]( const Server::WorldEvent& event ) {
        receivedPayload = event.payload();
    } );

    worldRuntime.combatSystem().resolveCharacterAttack( 1, 1, 0, 0, 30.0 );

    EXPECT_EQ( receivedPayload[ "x" ].asInt(), 1 );
    EXPECT_EQ( receivedPayload[ "y" ].asInt(), 0 );
}

TEST( WorldRuntimeTest, Tick_RegeneratesVitalsByOnePerSecond ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setMaxHealth( 100.0 );
    character->vitals().setHealth( 50.0 );
    character->vitals().setMaxMana( 100.0 );
    character->vitals().setMana( 50.0 );
    character->vitals().setMaxStamina( 100.0 );
    character->vitals().setStamina( 50.0 );

    for ( int tick = 0; tick < worldRuntime.tickRate() - 1; ++tick ) {
        worldRuntime.tick();
    }

    EXPECT_EQ( character->vitals().health(), 50.0 );

    worldRuntime.tick();

    EXPECT_EQ( character->vitals().health(), 51.0 );
    EXPECT_EQ( character->vitals().mana(), 51.0 );
    EXPECT_EQ( character->vitals().stamina(), 51.0 );
}

TEST( WorldRuntimeTest, Tick_RegenDoesNotExceedMaxVitals ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setMaxHealth( 100.0 );
    character->vitals().setHealth( 100.0 );
    character->vitals().setMaxMana( 100.0 );
    character->vitals().setMana( 99.5 );
    character->vitals().setMaxStamina( 100.0 );
    character->vitals().setStamina( 100.0 );

    for ( int tick = 0; tick < worldRuntime.tickRate() * 3; ++tick ) {
        worldRuntime.tick();
    }

    EXPECT_EQ( character->vitals().health(), 100.0 );
    EXPECT_EQ( character->vitals().mana(), 100.0 );
    EXPECT_EQ( character->vitals().stamina(), 100.0 );
}

TEST( WorldRuntimeTest, Tick_PublishesVitalsChanged_OnlyWhenRegenChangesVitals ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setMaxHealth( 100.0 );
    character->vitals().setHealth( 100.0 );
    character->vitals().setMaxMana( 100.0 );
    character->vitals().setMana( 100.0 );
    character->vitals().setMaxStamina( 100.0 );
    character->vitals().setStamina( 90.0 );

    int publishedCount = 0;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CHARACTER_VITALS_CHANGED, [ &publishedCount ]( const Server::WorldEvent& ) {
        ++publishedCount;
    } );

    for ( int tick = 0; tick < worldRuntime.tickRate() * 3; ++tick ) {
        worldRuntime.tick();
    }

    EXPECT_EQ( publishedCount, 3 );
}

namespace {

constexpr uint32_t AGGRESSIVE_CREATURE_TYPE = 900;
constexpr uint32_t SHORT_SIGHTED_CREATURE_TYPE = 901;
constexpr uint32_t PASSIVE_CREATURE_TYPE = 902;

void registerCreatureType( uint32_t type, uint32_t aggroRadius ) {
    Engine::CreatureTypeModel creatureType;
    creatureType.setType( type );
    creatureType.setName( "TestCreature" );
    creatureType.setAggroRadius( aggroRadius );

    Engine::Singleton<Engine::DataManager>::instance().addCreatureType( std::move( creatureType ) );
}

std::unique_ptr<Engine::CreatureModel> makeCreatureOfType( uint32_t type, int idCreature, int x, int y, int z ) {
    auto creature = makeCreature( idCreature, x, y, z );
    creature->setType( type );

    return creature;
}

void tickTimes( Server::WorldRuntime& worldRuntime, int ticks ) {
    for ( int tick = 0; tick < ticks; ++tick ) {
        worldRuntime.tick();
    }
}

} // namespace

TEST( WorldRuntimeTest, Attack_DoesNotHitBeforeCastEnds ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 100.0 );

    worldRuntime.combatSystem().attack( 1, 1, 0 );
    tickTimes( worldRuntime, 5 );

    EXPECT_DOUBLE_EQ( creature->vitals().health(), 100.0 );
}

TEST( WorldRuntimeTest, Attack_HitsTargetTileWhenCastEnds ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 100.0 );

    worldRuntime.combatSystem().attack( 1, 1, 0 );
    tickTimes( worldRuntime, worldRuntime.tickRate() );

    EXPECT_DOUBLE_EQ( creature->vitals().health(), 95.0 );
}

TEST( WorldRuntimeTest, Attack_MissesWhenTargetLeavesTileDuringCast ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 100.0 );

    worldRuntime.combatSystem().attack( 1, 1, 0 );
    creature->position().setX( 2 );
    tickTimes( worldRuntime, worldRuntime.tickRate() );

    EXPECT_DOUBLE_EQ( creature->vitals().health(), 100.0 );
}

TEST( WorldRuntimeTest, Attack_IsCancelledWhenAttackerMoves ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );
    Engine::CreatureModel* creature = worldRuntime.addCreature( makeCreature( 2, 1, 0, 0 ) );
    creature->vitals().setHealth( 100.0 );

    bool attackedPublished = false;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CHARACTER_ATTACKED, [ &attackedPublished ]( const Server::WorldEvent& ) {
        attackedPublished = true;
    } );

    worldRuntime.combatSystem().attack( 1, 1, 0 );
    worldRuntime.movementSystem().moveCharacter( 1, 0, 1, 0 );
    tickTimes( worldRuntime, worldRuntime.tickRate() );

    EXPECT_DOUBLE_EQ( creature->vitals().health(), 100.0 );
    EXPECT_FALSE( attackedPublished );
}

TEST( WorldRuntimeTest, Attack_WhileAlreadyCasting_IsBlocked ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );

    ASSERT_TRUE( worldRuntime.combatSystem().attack( 1, 1, 0 ) );
    character->combat().setCounter( 1000 );

    EXPECT_FALSE( worldRuntime.combatSystem().attack( 1, 1, 0 ) );
}

TEST( WorldRuntimeTest, Attack_PublishesAttackStartedWithTargetTileAndCastSeconds ) {
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setStamina( 100.0 );

    Json::Value receivedPayload;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CHARACTER_ATTACK_STARTED, [ &receivedPayload ]( const Server::WorldEvent& event ) {
        receivedPayload = event.payload();
    } );

    worldRuntime.combatSystem().attack( 1, 0, 1 );

    EXPECT_EQ( receivedPayload[ "idCharacter" ].asInt(), 1 );
    EXPECT_EQ( receivedPayload[ "x" ].asInt(), 0 );
    EXPECT_EQ( receivedPayload[ "y" ].asInt(), 1 );
    EXPECT_GT( receivedPayload[ "castSeconds" ].asDouble(), 0.0 );
}

TEST( WorldRuntimeTest, Tick_AggressiveCreatureAdjacentToCharacter_HitsAfterCast ) {
    registerCreatureType( AGGRESSIVE_CREATURE_TYPE, 5 );
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setMaxHealth( 100.0 );
    character->vitals().setHealth( 100.0 );
    worldRuntime.addCreature( makeCreatureOfType( AGGRESSIVE_CREATURE_TYPE, 2, 1, 0, 0 ) );

    tickTimes( worldRuntime, worldRuntime.tickRate() * 2 );

    EXPECT_LT( character->vitals().health(), 100.0 );
}

TEST( WorldRuntimeTest, Tick_AggressiveCreature_CharacterDodgesByLeavingTile ) {
    registerCreatureType( AGGRESSIVE_CREATURE_TYPE, 5 );
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setMaxHealth( 100.0 );
    character->vitals().setHealth( 100.0 );
    worldRuntime.addCreature( makeCreatureOfType( AGGRESSIVE_CREATURE_TYPE, 2, 1, 0, 0 ) );

    bool attackStarted = false;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CREATURE_ATTACK_STARTED, [ &worldRuntime, &attackStarted ]( const Server::WorldEvent& ) {
        attackStarted = true;
        worldRuntime.movementSystem().moveCharacter( 1, 0, 5, 0 );
    } );

    tickTimes( worldRuntime, worldRuntime.tickRate() * 2 );

    ASSERT_TRUE( attackStarted );
    EXPECT_DOUBLE_EQ( character->vitals().health(), 100.0 );
}

TEST( WorldRuntimeTest, Tick_AggressiveCreature_IgnoresCharacterOutsideAggroRadius ) {
    registerCreatureType( SHORT_SIGHTED_CREATURE_TYPE, 2 );
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setMaxHealth( 100.0 );
    character->vitals().setHealth( 100.0 );
    worldRuntime.addCreature( makeCreatureOfType( SHORT_SIGHTED_CREATURE_TYPE, 2, 5, 0, 0 ) );

    bool attackStarted = false;
    worldRuntime.eventBus().subscribe( Server::WorldEventType::CREATURE_ATTACK_STARTED, [ &attackStarted ]( const Server::WorldEvent& ) {
        attackStarted = true;
    } );

    tickTimes( worldRuntime, worldRuntime.tickRate() * 2 );

    EXPECT_FALSE( attackStarted );
    EXPECT_DOUBLE_EQ( character->vitals().health(), 100.0 );
}

TEST( WorldRuntimeTest, Tick_PassiveCreature_NeverAttacks ) {
    registerCreatureType( PASSIVE_CREATURE_TYPE, 0 );
    Server::WorldRuntime worldRuntime( nullptr );
    Engine::CharacterModel* character = worldRuntime.addCharacter( makeCharacter( 1, 0, 0, 0 ) );
    character->vitals().setMaxHealth( 100.0 );
    character->vitals().setHealth( 100.0 );
    worldRuntime.addCreature( makeCreatureOfType( PASSIVE_CREATURE_TYPE, 2, 1, 0, 0 ) );

    tickTimes( worldRuntime, worldRuntime.tickRate() * 3 );

    EXPECT_DOUBLE_EQ( character->vitals().health(), 100.0 );
}
