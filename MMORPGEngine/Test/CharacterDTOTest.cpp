#include <gtest/gtest.h>

#include <MMORPGEngine/Entity/Character/CharacterDTO.h>

TEST( CharacterDTOTest, ToJson_ThenFromJson_RoundTrips ) {
    Engine::CharacterDTO original;
    original.setIdCharacter( 7 );
    original.setX( 10 );
    original.setY( 20 );
    original.setZ( 1 );
    original.setHealth( 50.0 );
    original.setMaxHealth( 100.0 );
    original.setMana( 30.0 );
    original.setMaxMana( 60.0 );
    original.setStamina( 40.0 );
    original.setMaxStamina( 80.0 );

    const Engine::CharacterDTO restored = Engine::CharacterDTO::fromJson( original.toJson() );

    EXPECT_EQ( restored.idCharacter(), 7 );
    EXPECT_EQ( restored.x(), 10 );
    EXPECT_EQ( restored.y(), 20 );
    EXPECT_EQ( restored.z(), 1 );
    EXPECT_EQ( restored.health(), 50.0 );
    EXPECT_EQ( restored.maxHealth(), 100.0 );
    EXPECT_EQ( restored.mana(), 30.0 );
    EXPECT_EQ( restored.maxMana(), 60.0 );
    EXPECT_EQ( restored.stamina(), 40.0 );
    EXPECT_EQ( restored.maxStamina(), 80.0 );
}

TEST( CharacterDTOTest, FromJson_EmptyObject_KeepsDefaults ) {
    const Engine::CharacterDTO dto = Engine::CharacterDTO::fromJson( Json::Value( Json::objectValue ) );

    EXPECT_EQ( dto.idCharacter(), 0 );
    EXPECT_EQ( dto.x(), 0 );
    EXPECT_EQ( dto.y(), 0 );
    EXPECT_EQ( dto.z(), 0 );
    EXPECT_EQ( dto.health(), 0.0 );
    EXPECT_EQ( dto.maxHealth(), 0.0 );
}

TEST( CharacterDTOTest, FromJson_WrongFieldType_IsIgnored ) {
    Json::Value json;
    json[ "x" ] = "not-a-number";
    json[ "health" ] = "not-a-number";

    const Engine::CharacterDTO dto = Engine::CharacterDTO::fromJson( json );

    EXPECT_EQ( dto.x(), 0 );
    EXPECT_EQ( dto.health(), 0.0 );
}

TEST( CharacterDTOTest, ToJson_SetsMessageType ) {
    Engine::CharacterDTO dto;

    EXPECT_EQ( dto.toJson()[ "type" ].asString(), "character" );
}
