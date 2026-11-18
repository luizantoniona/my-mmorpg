#include <gtest/gtest.h>

#include <filesystem>
#include <functional>
#include <string>

#include <QImage>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Character/CharacterFactory.h>

namespace {

constexpr int FRAME_WIDTH = 4;
constexpr int FRAME_HEIGHT = 2;
constexpr int IDLE_FRAMES = 3;
constexpr int DIRECTIONS = 4;

class CharacterFactoryTestBase : public ::testing::Test {
protected:
    void SetUp() override {
        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_characterfactory";
        std::filesystem::create_directories( _tempDir / "TestMap" / "Characters" / "HERO" );

        Json::Value configJson;
        configJson[ "ActiveFolder" ] = "TestMap";
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "Config.json" ).string(), configJson );

        writeMap( true );

        writeSheet( "body_idle.png", IDLE_FRAMES * FRAME_WIDTH );
        writeSheet( "head_idle.png", IDLE_FRAMES * FRAME_WIDTH );
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    void writeMap( bool withCharactersCatalog ) const {
        Json::Value mapJson;
        mapJson[ "Catalogs" ][ "Tiles" ] = "Tiles.json";

        if ( withCharactersCatalog ) {
            mapJson[ "Catalogs" ][ "Characters" ] = "Characters.json";
        }

        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Map.json" ).string(), mapJson );
    }

    void writeSheet( const std::string& file, int width ) const {
        QImage sheet( width, DIRECTIONS * FRAME_HEIGHT, QImage::Format_ARGB32 );
        sheet.fill( Qt::red );
        sheet.save( QString::fromStdString( ( _tempDir / "TestMap" / "Characters" / "HERO" / file ).string() ) );
    }

    void writeCatalog( const Json::Value& catalogJson ) const {
        Engine::JsonHelper::saveJsonFile( ( _tempDir / "TestMap" / "Characters.json" ).string(), catalogJson );
    }

    static Json::Value validCatalog() {
        Json::Value json;
        json[ "FrameSize" ][ 0 ] = FRAME_WIDTH;
        json[ "FrameSize" ][ 1 ] = FRAME_HEIGHT;
        json[ "Rows" ][ "north" ] = 0;
        json[ "Rows" ][ "west" ] = 1;
        json[ "Rows" ][ "south" ] = 2;
        json[ "Rows" ][ "east" ] = 3;

        Json::Value& body = json[ "Bodies" ][ "HERO" ];
        body[ "Name" ] = "Hero";
        body[ "Folder" ] = "HERO";
        body[ "Animations" ][ "idle" ][ "Count" ] = IDLE_FRAMES;
        body[ "Animations" ][ "idle" ][ "FrameDurationMs" ] = 100;
        body[ "Layers" ][ "BODY" ][ "idle" ] = "body_idle.png";
        body[ "Layers" ][ "HEAD" ][ "idle" ] = "head_idle.png";

        return json;
    }

    Engine::CharacterCatalog createCatalog() const {
        Engine::CharacterCatalog catalog;
        Engine::CharacterFactory::createCharacterCatalog( QString::fromStdString( _tempDir.string() ) + "/", catalog );

        return catalog;
    }

    std::filesystem::path _tempDir;
};

class CharacterFactoryTest : public CharacterFactoryTestBase {};

class InvalidBodyCase {
public:
    InvalidBodyCase( std::string name, std::function<void( Json::Value& )> mutate ) :
        name( std::move( name ) ),
        mutate( std::move( mutate ) ) {
    }

    std::string name;
    std::function<void( Json::Value& )> mutate;
};

class InvalidBodyTest : public CharacterFactoryTestBase, public ::testing::WithParamInterface<InvalidBodyCase> {};

TEST( CharacterFactoryIdTest, IsValidId_UppercaseDigitsAndUnderscore_ReturnsTrue ) {
    EXPECT_TRUE( Engine::CharacterFactory::isValidId( "FEMININE_HONEY_2" ) );
}

TEST( CharacterFactoryIdTest, IsValidId_LowercaseOrSpace_ReturnsFalse ) {
    EXPECT_FALSE( Engine::CharacterFactory::isValidId( "hero" ) );
    EXPECT_FALSE( Engine::CharacterFactory::isValidId( "HERO ONE" ) );
    EXPECT_FALSE( Engine::CharacterFactory::isValidId( "" ) );
}

TEST_F( CharacterFactoryTest, Create_ValidCatalog_LoadsHeaderAndBody ) {
    writeCatalog( validCatalog() );

    const Engine::CharacterCatalog catalog = createCatalog();

    EXPECT_EQ( catalog.frameSize(), QSize( FRAME_WIDTH, FRAME_HEIGHT ) );
    EXPECT_EQ( catalog.rows().size(), 4u );
    EXPECT_EQ( catalog.rowCount(), DIRECTIONS );

    const Engine::CharacterBodyModel* body = catalog.body( "HERO" );
    ASSERT_NE( body, nullptr );
    EXPECT_EQ( body->name(), "Hero" );
    EXPECT_TRUE( body->sprite().folder().endsWith( "TestMap/Characters/HERO/" ) );
    EXPECT_EQ( body->sprite().file( Engine::CharacterLayerEnum::BODY, Engine::AppearanceStateEnum::IDLE ), "body_idle.png" );
    EXPECT_EQ( body->sprite().file( Engine::CharacterLayerEnum::HEAD, Engine::AppearanceStateEnum::IDLE ), "head_idle.png" );

    const Engine::CharacterClipModel* clip = body->clip( Engine::AppearanceStateEnum::IDLE );
    ASSERT_NE( clip, nullptr );
    EXPECT_EQ( clip->frameCount(), IDLE_FRAMES );
    EXPECT_EQ( clip->frameDurationMs(), 100 );
}

TEST_F( CharacterFactoryTest, Create_NoCharactersEntryInMap_LeavesCatalogEmpty ) {
    writeCatalog( validCatalog() );
    writeMap( false );

    EXPECT_TRUE( createCatalog().isEmpty() );
}

TEST_F( CharacterFactoryTest, Create_CatalogFileMissing_LeavesCatalogEmpty ) {
    EXPECT_TRUE( createCatalog().isEmpty() );
}

TEST_F( CharacterFactoryTest, Create_RowsWithoutAllDirections_LeavesCatalogEmpty ) {
    Json::Value json = validCatalog();
    json[ "Rows" ].removeMember( "east" );
    writeCatalog( json );

    EXPECT_TRUE( createCatalog().isEmpty() );
}

TEST_F( CharacterFactoryTest, Create_InvalidFrameSize_LeavesCatalogEmpty ) {
    Json::Value json = validCatalog();
    json[ "FrameSize" ][ 0 ] = 0;
    writeCatalog( json );

    EXPECT_TRUE( createCatalog().isEmpty() );
}

TEST_F( CharacterFactoryTest, Create_UnknownAnimationState_IgnoresOnlyThatState ) {
    Json::Value json = validCatalog();
    json[ "Bodies" ][ "HERO" ][ "Animations" ][ "fly" ][ "Count" ] = 2;
    json[ "Bodies" ][ "HERO" ][ "Animations" ][ "fly" ][ "FrameDurationMs" ] = 100;
    writeCatalog( json );

    const Engine::CharacterCatalog catalog = createCatalog();

    const Engine::CharacterBodyModel* body = catalog.body( "HERO" );
    ASSERT_NE( body, nullptr );
    EXPECT_EQ( body->clips().size(), 1u );
}

TEST_F( CharacterFactoryTest, Create_HeadSheetWithWrongWidth_SkipsTheBody ) {
    writeSheet( "head_idle.png", ( IDLE_FRAMES + 1 ) * FRAME_WIDTH );
    writeCatalog( validCatalog() );

    EXPECT_TRUE( createCatalog().isEmpty() );
}

TEST_P( InvalidBodyTest, Create_InvalidBody_IsSkipped ) {
    Json::Value json = validCatalog();
    GetParam().mutate( json );
    writeCatalog( json );

    EXPECT_TRUE( createCatalog().isEmpty() );
}

INSTANTIATE_TEST_SUITE_P(
    CharacterFactory,
    InvalidBodyTest,
    ::testing::Values(
        InvalidBodyCase( "LowercaseId", []( Json::Value& json ) {
            json[ "Bodies" ][ "hero" ] = json[ "Bodies" ][ "HERO" ];
            json[ "Bodies" ].removeMember( "HERO" );
        } ),
        InvalidBodyCase( "FolderOutsideTheMap", []( Json::Value& json ) { json[ "Bodies" ][ "HERO" ][ "Folder" ] = "../HERO"; } ), InvalidBodyCase( "MissingName", []( Json::Value& json ) { json[ "Bodies" ][ "HERO" ].removeMember( "Name" ); } ), InvalidBodyCase( "NameWithWrongType", []( Json::Value& json ) { json[ "Bodies" ][ "HERO" ][ "Name" ] = 5; } ), InvalidBodyCase( "WithoutBodyLayer", []( Json::Value& json ) { json[ "Bodies" ][ "HERO" ][ "Layers" ].removeMember( "BODY" ); } ), InvalidBodyCase( "WithoutHeadLayer", []( Json::Value& json ) { json[ "Bodies" ][ "HERO" ][ "Layers" ].removeMember( "HEAD" ); } ), InvalidBodyCase( "UnknownLayerOnly", []( Json::Value& json ) {
            json[ "Bodies" ][ "HERO" ][ "Layers" ][ "TAIL" ] = json[ "Bodies" ][ "HERO" ][ "Layers" ][ "BODY" ];
            json[ "Bodies" ][ "HERO" ][ "Layers" ].removeMember( "BODY" );
        } ),
        InvalidBodyCase( "BodySheetNotOnDisk", []( Json::Value& json ) { json[ "Bodies" ][ "HERO" ][ "Layers" ][ "BODY" ][ "idle" ] = "missing.png"; } ), InvalidBodyCase( "BodySheetWithWrongWidth", []( Json::Value& json ) { json[ "Bodies" ][ "HERO" ][ "Animations" ][ "idle" ][ "Count" ] = IDLE_FRAMES + 1; } ), InvalidBodyCase( "ZeroFrameCount", []( Json::Value& json ) { json[ "Bodies" ][ "HERO" ][ "Animations" ][ "idle" ][ "Count" ] = 0; } ) ),
    []( const ::testing::TestParamInfo<InvalidBodyCase>& info ) { return info.param.name; } );

} // namespace
