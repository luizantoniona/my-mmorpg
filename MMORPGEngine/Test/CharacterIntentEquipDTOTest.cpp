#include <gtest/gtest.h>

#include <string>

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

namespace {

class JsonFieldCase {
public:
    JsonFieldCase( std::string name, Json::Value value, bool expectedAccepted ) :
        name( std::move( name ) ),
        value( std::move( value ) ),
        expectedAccepted( expectedAccepted ) {
    }

    std::string name;
    Json::Value value;
    bool expectedAccepted;
};

class CharacterIntentEquipDTOPositionTest : public ::testing::TestWithParam<JsonFieldCase> {};
class CharacterIntentUnequipDTOSlotTest : public ::testing::TestWithParam<JsonFieldCase> {};

std::string jsonFieldCaseName( const ::testing::TestParamInfo<JsonFieldCase>& info ) {
    return info.param.name;
}

Json::Value objectWith( const char* key, const Json::Value& value ) {
    Json::Value json( Json::objectValue );
    json[ key ] = value;

    return json;
}

} // namespace

TEST_P( CharacterIntentEquipDTOPositionTest, FromJson_PositionIsAcceptedOnlyWhenUnsignedInteger ) {
    const Engine::CharacterIntentEquipDTO dto = Engine::CharacterIntentEquipDTO::fromJson( objectWith( "position", GetParam().value ) );

    EXPECT_EQ( dto.position().has_value(), GetParam().expectedAccepted );
}

INSTANTIATE_TEST_SUITE_P(
    Positions,
    CharacterIntentEquipDTOPositionTest,
    ::testing::Values(
        JsonFieldCase( "Zero_IsAccepted", Json::Value( 0 ), true ),
        JsonFieldCase( "Positive_IsAccepted", Json::Value( 7 ), true ),
        JsonFieldCase( "Negative_IsRejected", Json::Value( -1 ), false ),
        JsonFieldCase( "Text_IsRejected", Json::Value( "first" ), false ),
        JsonFieldCase( "Fraction_IsRejected", Json::Value( 1.5 ), false ),
        JsonFieldCase( "Boolean_IsRejected", Json::Value( true ), false ),
        JsonFieldCase( "Null_IsRejected", Json::Value( Json::nullValue ), false ) ),
    jsonFieldCaseName );

TEST( CharacterIntentEquipDTOTest, FromJson_MissingPosition_HasNoPosition ) {
    EXPECT_FALSE( Engine::CharacterIntentEquipDTO::fromJson( Json::Value( Json::objectValue ) ).position().has_value() );
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

TEST_P( CharacterIntentUnequipDTOSlotTest, FromJson_SlotIsAcceptedOnlyWhenKnownName ) {
    const Engine::CharacterIntentUnequipDTO dto = Engine::CharacterIntentUnequipDTO::fromJson( objectWith( "slot", GetParam().value ) );

    EXPECT_EQ( dto.slot().has_value(), GetParam().expectedAccepted );
}

INSTANTIATE_TEST_SUITE_P(
    Slots,
    CharacterIntentUnequipDTOSlotTest,
    ::testing::Values(
        JsonFieldCase( "LeftHand_IsAccepted", Json::Value( "LEFT_HAND" ), true ),
        JsonFieldCase( "RightHand_IsAccepted", Json::Value( "RIGHT_HAND" ), true ),
        JsonFieldCase( "UnknownName_IsRejected", Json::Value( "BACKPACK" ), false ),
        JsonFieldCase( "EmptyText_IsRejected", Json::Value( "" ), false ),
        JsonFieldCase( "Number_IsRejected", Json::Value( 3 ), false ),
        JsonFieldCase( "Null_IsRejected", Json::Value( Json::nullValue ), false ) ),
    jsonFieldCaseName );

TEST( CharacterIntentUnequipDTOTest, FromJson_MissingSlot_HasNoSlot ) {
    EXPECT_FALSE( Engine::CharacterIntentUnequipDTO::fromJson( Json::Value( Json::objectValue ) ).slot().has_value() );
}
