#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include <MMORPGEngine/Commons/JsonHelper.h>

namespace {

class FieldTypeCase {
public:
    FieldTypeCase( std::string name, Engine::JsonFieldTypeEnum type, Json::Value value, bool expectedValid ) :
        name( std::move( name ) ),
        type( type ),
        value( std::move( value ) ),
        expectedValid( expectedValid ) {
    }

    std::string name;
    Engine::JsonFieldTypeEnum type;
    Json::Value value;
    bool expectedValid;
};

class JsonHelperFieldTypeTest : public ::testing::TestWithParam<FieldTypeCase> {};

Json::Value arrayOfOne() {
    Json::Value array( Json::arrayValue );
    array.append( 1 );

    return array;
}

} // namespace

TEST_P( JsonHelperFieldTypeTest, HasValidFields_ChecksFieldType ) {
    Json::Value json( Json::objectValue );
    json[ "field" ] = GetParam().value;

    EXPECT_EQ( Engine::JsonHelper::hasValidFields( json, { { "field", GetParam().type } } ), GetParam().expectedValid );
}

INSTANTIATE_TEST_SUITE_P(
    Types,
    JsonHelperFieldTypeTest,
    ::testing::Values(
        FieldTypeCase( "String_Text_IsValid", Engine::JsonFieldTypeEnum::STRING, Json::Value( "a" ), true ),
        FieldTypeCase( "String_Number_IsInvalid", Engine::JsonFieldTypeEnum::STRING, Json::Value( 5 ), false ),
        FieldTypeCase( "Uint_Zero_IsValid", Engine::JsonFieldTypeEnum::UINT, Json::Value( 0 ), true ),
        FieldTypeCase( "Uint_Negative_IsInvalid", Engine::JsonFieldTypeEnum::UINT, Json::Value( -1 ), false ),
        FieldTypeCase( "Uint_Text_IsInvalid", Engine::JsonFieldTypeEnum::UINT, Json::Value( "1" ), false ),
        FieldTypeCase( "Uint_Fraction_IsInvalid", Engine::JsonFieldTypeEnum::UINT, Json::Value( 1.5 ), false ),
        FieldTypeCase( "Int_Negative_IsValid", Engine::JsonFieldTypeEnum::INT, Json::Value( -3 ), true ),
        FieldTypeCase( "Int_Boolean_IsInvalid", Engine::JsonFieldTypeEnum::INT, Json::Value( true ), false ),
        FieldTypeCase( "Number_Fraction_IsValid", Engine::JsonFieldTypeEnum::NUMBER, Json::Value( 1.5 ), true ),
        FieldTypeCase( "Number_Text_IsInvalid", Engine::JsonFieldTypeEnum::NUMBER, Json::Value( "1.5" ), false ),
        FieldTypeCase( "Bool_Boolean_IsValid", Engine::JsonFieldTypeEnum::BOOL, Json::Value( false ), true ),
        FieldTypeCase( "Bool_Number_IsInvalid", Engine::JsonFieldTypeEnum::BOOL, Json::Value( 0 ), false ),
        FieldTypeCase( "Array_Array_IsValid", Engine::JsonFieldTypeEnum::ARRAY, arrayOfOne(), true ),
        FieldTypeCase( "Array_Object_IsInvalid", Engine::JsonFieldTypeEnum::ARRAY, Json::Value( Json::objectValue ), false ),
        FieldTypeCase( "Object_Object_IsValid", Engine::JsonFieldTypeEnum::OBJECT, Json::Value( Json::objectValue ), true ),
        FieldTypeCase( "Object_Array_IsInvalid", Engine::JsonFieldTypeEnum::OBJECT, arrayOfOne(), false ),
        FieldTypeCase( "AnyType_Null_IsInvalid", Engine::JsonFieldTypeEnum::STRING, Json::Value( Json::nullValue ), false ) ),
    []( const ::testing::TestParamInfo<FieldTypeCase>& info ) { return info.param.name; } );

TEST( JsonHelperTest, HasValidFields_AbsentField_IsValid ) {
    EXPECT_TRUE( Engine::JsonHelper::hasValidFields( Json::Value( Json::objectValue ), { { "missing", Engine::JsonFieldTypeEnum::STRING } } ) );
}

TEST( JsonHelperTest, HasValidFields_NotAnObject_IsInvalid ) {
    EXPECT_FALSE( Engine::JsonHelper::hasValidFields( Json::Value( 5 ), { { "field", Engine::JsonFieldTypeEnum::STRING } } ) );
    EXPECT_FALSE( Engine::JsonHelper::hasValidFields( Json::Value( Json::arrayValue ), { { "field", Engine::JsonFieldTypeEnum::STRING } } ) );
}

TEST( JsonHelperTest, HasValidFields_OneWrongAmongSeveral_IsInvalid ) {
    Json::Value json( Json::objectValue );
    json[ "name" ] = "Rat";
    json[ "level" ] = "three";

    EXPECT_FALSE( Engine::JsonHelper::hasValidFields( json, { { "name", Engine::JsonFieldTypeEnum::STRING }, { "level", Engine::JsonFieldTypeEnum::UINT } } ) );
}

TEST( JsonHelperTest, ParseJsonString_ValidJson_ReturnsExpectedValues ) {
    const Json::Value json = Engine::JsonHelper::parseJsonString( std::string( R"({"name":"Rat","level":3})" ) );

    EXPECT_EQ( json[ "name" ].asString(), "Rat" );
    EXPECT_EQ( json[ "level" ].asInt(), 3 );
}

TEST( JsonHelperTest, ParseJsonString_InvalidJson_ReturnsNull ) {
    const Json::Value json = Engine::JsonHelper::parseJsonString( std::string( "{not valid json" ) );

    EXPECT_TRUE( json.isNull() );
}

TEST( JsonHelperTest, WriteJsonString_ThenParse_RoundTrips ) {
    Json::Value original;
    original[ "name" ] = "Rat";
    original[ "level" ] = 3;

    const std::string serialized = Engine::JsonHelper::writeJsonString( original );
    const Json::Value restored = Engine::JsonHelper::parseJsonString( serialized );

    EXPECT_EQ( restored[ "name" ].asString(), "Rat" );
    EXPECT_EQ( restored[ "level" ].asInt(), 3 );
}

TEST( JsonHelperTest, SaveJsonFile_ThenLoad_RoundTrips ) {
    const std::string path = ( std::filesystem::temp_directory_path() / "mmorpg_engine_test_jsonhelper.json" ).string();

    Json::Value original;
    original[ "name" ] = "Rat";
    original[ "level" ] = 3;

    ASSERT_TRUE( Engine::JsonHelper::saveJsonFile( path, original ) );

    const Json::Value restored = Engine::JsonHelper::loadJsonFile( path );

    EXPECT_EQ( restored[ "name" ].asString(), "Rat" );
    EXPECT_EQ( restored[ "level" ].asInt(), 3 );

    std::filesystem::remove( path );
}

TEST( JsonHelperTest, LoadJsonFile_NonexistentFile_ReturnsNull ) {
    const Json::Value json = Engine::JsonHelper::loadJsonFile( std::string( "mmorpg_engine_test_definitely_missing.json" ) );

    EXPECT_TRUE( json.isNull() );
}

TEST( JsonHelperTest, SaveJsonFile_NonexistentDirectory_ReturnsFalse ) {
    const bool saved = Engine::JsonHelper::saveJsonFile( std::string( "mmorpg_engine_test_missing_dir_xyz/file.json" ), Json::Value() );

    EXPECT_FALSE( saved );
}
