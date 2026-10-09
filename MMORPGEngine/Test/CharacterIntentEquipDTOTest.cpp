#include <gtest/gtest.h>

#include <MMORPGEngine/Entity/Character/CharacterIntentEquipDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterIntentUnequipDTO.h>
#include <MMORPGEngine/Network/WebSocket/ClientMessageTypeHelper.h>

TEST( CharacterIntentEquipDTOTest, ToJsonThenFromJson_RoundTripsPosition ) {
    Engine::CharacterIntentEquipDTO dto;
    dto.setPosition( 3 );

    const Json::Value json = dto.toJson();
    const Engine::CharacterIntentEquipDTO reloaded = Engine::CharacterIntentEquipDTO::fromJson( json );

    EXPECT_EQ( Engine::ClientMessageTypeHelper::fromMessage( json ), Engine::ClientMessageType::CHARACTER_INTENT_EQUIP );
    ASSERT_TRUE( reloaded.position().has_value() );
    EXPECT_EQ( reloaded.position().value(), 3u );
}

TEST( CharacterIntentEquipDTOTest, FromJson_MissingOrInvalidPosition_HasNoPosition ) {
    Json::Value negative;
    negative[ "position" ] = -1;

    Json::Value text;
    text[ "position" ] = "first";

    EXPECT_FALSE( Engine::CharacterIntentEquipDTO::fromJson( Json::Value( Json::objectValue ) ).position().has_value() );
    EXPECT_FALSE( Engine::CharacterIntentEquipDTO::fromJson( negative ).position().has_value() );
    EXPECT_FALSE( Engine::CharacterIntentEquipDTO::fromJson( text ).position().has_value() );
}

TEST( CharacterIntentUnequipDTOTest, ToJsonThenFromJson_RoundTripsSlot ) {
    Engine::CharacterIntentUnequipDTO dto;
    dto.setSlot( Engine::EquipmentSlotEnum::LEFT_HAND );

    const Json::Value json = dto.toJson();
    const Engine::CharacterIntentUnequipDTO reloaded = Engine::CharacterIntentUnequipDTO::fromJson( json );

    EXPECT_EQ( Engine::ClientMessageTypeHelper::fromMessage( json ), Engine::ClientMessageType::CHARACTER_INTENT_UNEQUIP );
    ASSERT_TRUE( reloaded.slot().has_value() );
    EXPECT_EQ( reloaded.slot().value(), Engine::EquipmentSlotEnum::LEFT_HAND );
}

TEST( CharacterIntentUnequipDTOTest, FromJson_MissingOrUnknownSlot_HasNoSlot ) {
    Json::Value unknown;
    unknown[ "slot" ] = "BACKPACK";

    EXPECT_FALSE( Engine::CharacterIntentUnequipDTO::fromJson( Json::Value( Json::objectValue ) ).slot().has_value() );
    EXPECT_FALSE( Engine::CharacterIntentUnequipDTO::fromJson( unknown ).slot().has_value() );
}
