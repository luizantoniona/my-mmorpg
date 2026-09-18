#include <gtest/gtest.h>

#include <MMORPGServer/Server/Event/EventBus.h>

TEST( EventBusTest, Publish_NoSubscribers_DoesNotCrash ) {
    Server::EventBus eventBus;

    Json::Value payload;
    payload[ "idCharacter" ] = 1;

    EXPECT_NO_THROW( eventBus.publish( Server::WorldEvent( Server::WorldEventType::ENTITY_MOVED, payload ) ) );
}

TEST( EventBusTest, Publish_NotifiesSubscriberOfMatchingType ) {
    Server::EventBus eventBus;

    int receivedIdCharacter = 0;
    eventBus.subscribe( Server::WorldEventType::ENTITY_MOVED, [ &receivedIdCharacter ]( const Server::WorldEvent& event ) {
        receivedIdCharacter = event.payload()[ "idCharacter" ].asInt();
    } );

    Json::Value payload;
    payload[ "idCharacter" ] = 7;
    eventBus.publish( Server::WorldEvent( Server::WorldEventType::ENTITY_MOVED, payload ) );

    EXPECT_EQ( receivedIdCharacter, 7 );
}

TEST( EventBusTest, Publish_DoesNotNotifySubscriberOfDifferentType ) {
    Server::EventBus eventBus;

    bool called = false;
    eventBus.subscribe( Server::WorldEventType::ENTITY_LEFT, [ &called ]( const Server::WorldEvent& ) {
        called = true;
    } );

    Json::Value payload;
    eventBus.publish( Server::WorldEvent( Server::WorldEventType::ENTITY_MOVED, payload ) );

    EXPECT_FALSE( called );
}

TEST( EventBusTest, Publish_NotifiesAllSubscribersOfSameType ) {
    Server::EventBus eventBus;

    int callCount = 0;
    eventBus.subscribe( Server::WorldEventType::ENTITY_ENTERED, [ &callCount ]( const Server::WorldEvent& ) {
        ++callCount;
    } );
    eventBus.subscribe( Server::WorldEventType::ENTITY_ENTERED, [ &callCount ]( const Server::WorldEvent& ) {
        ++callCount;
    } );

    Json::Value payload;
    eventBus.publish( Server::WorldEvent( Server::WorldEventType::ENTITY_ENTERED, payload ) );

    EXPECT_EQ( callCount, 2 );
}
