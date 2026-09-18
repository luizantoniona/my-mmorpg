#include <gtest/gtest.h>

#include <filesystem>

#include <MMORPGEngine/Commons/JsonHelper.h>

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
