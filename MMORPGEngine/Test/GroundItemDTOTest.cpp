#include <gtest/gtest.h>

#include <string>

#include <MMORPGEngine/Entity/Character/CharacterIntentDropDTO.h>
#include <MMORPGEngine/Entity/Character/CharacterIntentPickupDTO.h>
#include <MMORPGEngine/Entity/GroundItem/GroundItemDTO.h>
#include <MMORPGEngine/Entity/GroundItem/GroundItemEventLeaveDTO.h>
#include <MMORPGEngine/Network/WebSocket/ClientMessageTypeHelper.h>
#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

TEST( GroundItemDTOTest, FromModel_ThenToJsonThenFromJson_RoundTripsFields ) {
    Engine::GroundItemModel model;
    model.setIdGroundItem( 7 );
    model.setIdItem( "IRON_SWORD" );
    model.position().setX( 10 );
    model.position().setY( 20 );
    model.position().setZ( 1 );

    const Json::Value json = Engine::GroundItemDTO::fromModel( &model ).toJson();
    const Engine::GroundItemDTO reloaded = Engine::GroundItemDTO::fromJson( json );

    EXPECT_EQ( Engine::ServerMessageTypeHelper::fromMessage( json ), Engine::ServerMessageType::GROUND_ITEM );
    EXPECT_EQ( reloaded.idGroundItem(), 7 );
    EXPECT_EQ( reloaded.idItem(), "IRON_SWORD" );
    EXPECT_EQ( reloaded.x(), 10 );
    EXPECT_EQ( reloaded.y(), 20 );
    EXPECT_EQ( reloaded.z(), 1 );
}

TEST( GroundItemDTOTest, FromJson_NumericIdItem_IsIgnored ) {
    Json::Value json;
    json[ "idGroundItem" ] = 7;
    json[ "idItem" ] = 42;

    EXPECT_TRUE( Engine::GroundItemDTO::fromJson( json ).idItem().isEmpty() );
}

TEST( GroundItemEventLeaveDTOTest, ToJsonThenFromJson_RoundTripsId ) {
    Engine::GroundItemEventLeaveDTO dto;
    dto.setIdGroundItem( 9 );

    const Json::Value json = dto.toJson();

    EXPECT_EQ( Engine::ServerMessageTypeHelper::fromMessage( json ), Engine::ServerMessageType::GROUND_ITEM_EVENT_LEAVE );
    EXPECT_EQ( Engine::GroundItemEventLeaveDTO::fromJson( json ).idGroundItem(), 9 );
}

TEST( CharacterIntentDropDTOTest, ToJsonThenFromJson_RoundTripsPosition ) {
    Engine::CharacterIntentDropDTO dto;
    dto.setPosition( 4 );

    const Json::Value json = dto.toJson();
    const Engine::CharacterIntentDropDTO reloaded = Engine::CharacterIntentDropDTO::fromJson( json );

    EXPECT_EQ( Engine::ClientMessageTypeHelper::fromMessage( json ), Engine::ClientMessageType::CHARACTER_INTENT_DROP );
    ASSERT_TRUE( reloaded.position().has_value() );
    EXPECT_EQ( reloaded.position().value(), 4u );
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

class CharacterIntentDropDTOPositionTest : public ::testing::TestWithParam<JsonFieldCase> {};
class CharacterIntentPickupDTOIdTest : public ::testing::TestWithParam<JsonFieldCase> {};

std::string jsonFieldCaseName( const ::testing::TestParamInfo<JsonFieldCase>& info ) {
    return info.param.name;
}

Json::Value objectWith( const char* key, const Json::Value& value ) {
    Json::Value json( Json::objectValue );
    json[ key ] = value;

    return json;
}

} // namespace

TEST_P( CharacterIntentDropDTOPositionTest, FromJson_PositionIsAcceptedOnlyWhenUnsignedInteger ) {
    const Engine::CharacterIntentDropDTO dto = Engine::CharacterIntentDropDTO::fromJson( objectWith( "position", GetParam().value ) );

    EXPECT_EQ( dto.position().has_value(), GetParam().expectedAccepted );
}

INSTANTIATE_TEST_SUITE_P(
    Positions,
    CharacterIntentDropDTOPositionTest,
    ::testing::Values(
        JsonFieldCase( "Zero_IsAccepted", Json::Value( 0 ), true ),
        JsonFieldCase( "Positive_IsAccepted", Json::Value( 19 ), true ),
        JsonFieldCase( "Negative_IsRejected", Json::Value( -1 ), false ),
        JsonFieldCase( "Text_IsRejected", Json::Value( "3" ), false ),
        JsonFieldCase( "Fraction_IsRejected", Json::Value( 0.5 ), false ),
        JsonFieldCase( "Null_IsRejected", Json::Value( Json::nullValue ), false ) ),
    jsonFieldCaseName );

TEST( CharacterIntentDropDTOTest, FromJson_MissingPosition_HasNoPosition ) {
    EXPECT_FALSE( Engine::CharacterIntentDropDTO::fromJson( Json::Value( Json::objectValue ) ).position().has_value() );
}

TEST_P( CharacterIntentPickupDTOIdTest, FromJson_IdGroundItemIsAcceptedOnlyWhenInteger ) {
    const Engine::CharacterIntentPickupDTO dto = Engine::CharacterIntentPickupDTO::fromJson( objectWith( "idGroundItem", GetParam().value ) );

    EXPECT_EQ( dto.idGroundItem().has_value(), GetParam().expectedAccepted );
}

INSTANTIATE_TEST_SUITE_P(
    Ids,
    CharacterIntentPickupDTOIdTest,
    ::testing::Values(
        JsonFieldCase( "Positive_IsAccepted", Json::Value( 7 ), true ),
        JsonFieldCase( "Zero_IsAccepted", Json::Value( 0 ), true ),
        JsonFieldCase( "Text_IsRejected", Json::Value( "7" ), false ),
        JsonFieldCase( "Fraction_IsRejected", Json::Value( 7.5 ), false ),
        JsonFieldCase( "Boolean_IsRejected", Json::Value( true ), false ),
        JsonFieldCase( "Null_IsRejected", Json::Value( Json::nullValue ), false ) ),
    jsonFieldCaseName );

TEST( CharacterIntentPickupDTOTest, ToJsonThenFromJson_RoundTripsIdGroundItem ) {
    Engine::CharacterIntentPickupDTO dto;
    dto.setIdGroundItem( 5 );

    const Json::Value json = dto.toJson();
    const Engine::CharacterIntentPickupDTO reloaded = Engine::CharacterIntentPickupDTO::fromJson( json );

    EXPECT_EQ( Engine::ClientMessageTypeHelper::fromMessage( json ), Engine::ClientMessageType::CHARACTER_INTENT_PICKUP );
    ASSERT_TRUE( reloaded.idGroundItem().has_value() );
    EXPECT_EQ( reloaded.idGroundItem().value(), 5 );
}

TEST( CharacterIntentPickupDTOTest, FromJson_MissingIdGroundItem_HasNoIdGroundItem ) {
    EXPECT_FALSE( Engine::CharacterIntentPickupDTO::fromJson( Json::Value( Json::objectValue ) ).idGroundItem().has_value() );
}
