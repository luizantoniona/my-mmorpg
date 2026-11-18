#include <gtest/gtest.h>

#include <string>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Entity/Character/CharacterAppearanceDTO.h>

namespace {

class InvalidSelectionCase {
public:
    InvalidSelectionCase( std::string name, std::string json ) :
        name( std::move( name ) ),
        json( std::move( json ) ) {
    }

    std::string name;
    std::string json;
};

class InvalidSelectionTest : public ::testing::TestWithParam<InvalidSelectionCase> {};

TEST_P( InvalidSelectionTest, SelectionFromJson_Rejected ) {
    Engine::CharacterAppearanceModel appearance;
    appearance.setId( Engine::CharacterAppearanceFeatureEnum::BODY, "KEPT" );

    EXPECT_FALSE( Engine::CharacterAppearanceDTO::selectionFromJson( Engine::JsonHelper::parseJsonString( GetParam().json ), appearance ) );
    EXPECT_EQ( appearance.id( Engine::CharacterAppearanceFeatureEnum::BODY ), "KEPT" );
}

INSTANTIATE_TEST_SUITE_P(
    CharacterAppearanceDTO,
    InvalidSelectionTest,
    ::testing::Values(
        InvalidSelectionCase( "NotAnObject", R"(["BODY"])" ),
        InvalidSelectionCase( "UnknownFeature", R"({"BODY":"HERO","TAIL":"LONG"})" ),
        InvalidSelectionCase( "IdWithWrongType", R"({"BODY":1})" ),
        InvalidSelectionCase( "EmptyId", R"({"BODY":""})" ) ),
    []( const ::testing::TestParamInfo<InvalidSelectionCase>& info ) { return info.param.name; } );

TEST( CharacterAppearanceDTOTest, ToJson_ThenFromJson_RoundTrips ) {
    Engine::CharacterAppearanceModel appearance;
    appearance.setId( Engine::CharacterAppearanceFeatureEnum::BODY, "FEMININE_IVORY" );

    Engine::CharacterAppearanceDTO original;
    original.setIdCharacter( 7 );
    original.setAppearance( appearance );

    const Engine::CharacterAppearanceDTO restored = Engine::CharacterAppearanceDTO::fromJson( original.toJson() );

    EXPECT_EQ( restored.idCharacter(), 7 );
    EXPECT_EQ( restored.appearance().id( Engine::CharacterAppearanceFeatureEnum::BODY ), "FEMININE_IVORY" );
}

TEST( CharacterAppearanceDTOTest, ToJson_SetsMessageType ) {
    const Json::Value json = Engine::CharacterAppearanceDTO().toJson();

    EXPECT_EQ( json[ "type" ].asString(), "CHARACTER_APPEARANCE" );
}

TEST( CharacterAppearanceDTOTest, FromJson_InvalidAppearance_KeepsItEmpty ) {
    Json::Value json( Json::objectValue );
    json[ "idCharacter" ] = 3;
    json[ "appearance" ] = "BODY";

    const Engine::CharacterAppearanceDTO dto = Engine::CharacterAppearanceDTO::fromJson( json );

    EXPECT_EQ( dto.idCharacter(), 3 );
    EXPECT_TRUE( dto.appearance().isEmpty() );
}

TEST( CharacterAppearanceDTOTest, SelectionToJson_Empty_IsAnEmptyObject ) {
    const Json::Value json = Engine::CharacterAppearanceDTO::selectionToJson( Engine::CharacterAppearanceModel() );

    EXPECT_TRUE( json.isObject() );
    EXPECT_TRUE( json.empty() );
}

} // namespace
