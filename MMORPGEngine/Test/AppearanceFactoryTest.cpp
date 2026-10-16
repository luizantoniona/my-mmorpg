#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include <QImage>

#include <MMORPGEngine/Data/Appearance/AppearanceDirectionHelper.h>
#include <MMORPGEngine/Data/Appearance/AppearanceFactory.h>

namespace {

constexpr int FRAME_WIDTH = 4;
constexpr int FRAME_HEIGHT = 2;
constexpr int COLUMNS = 3;
constexpr int ROWS = 2;

class AppearanceFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_appearancefactory";
        std::filesystem::create_directories( _tempDir );

        writeSheet( "sheet.png", 0 );
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    void writeSheet( const std::string& file, int gap ) const {
        QImage sheet( COLUMNS * ( FRAME_WIDTH + gap ), ROWS * FRAME_HEIGHT, QImage::Format_ARGB32 );
        sheet.fill( Qt::transparent );

        for ( int row = 0; row < ROWS; ++row ) {
            for ( int column = 0; column < COLUMNS; ++column ) {
                for ( int y = 0; y < FRAME_HEIGHT; ++y ) {
                    for ( int x = 0; x < FRAME_WIDTH; ++x ) {
                        sheet.setPixel( column * ( FRAME_WIDTH + gap ) + x, row * FRAME_HEIGHT + y, cellColor( column, row ) );
                    }
                }
            }
        }

        sheet.save( QString::fromStdString( ( _tempDir / file ).string() ) );
    }

    static QRgb cellColor( int column, int row ) {
        return qRgba( 40 + column * 60, 40 + row * 100, 200, 255 );
    }

    static Json::Value clip( int from, int count, int southRow ) {
        Json::Value json;
        json[ "File" ] = "sheet.png";
        json[ "Rows" ][ "south" ] = southRow;
        json[ "From" ] = from;
        json[ "Count" ] = count;
        json[ "FrameDurationMs" ] = 100;
        return json;
    }

    static Json::Value appearance( const std::string& state, const Json::Value& clipJson ) {
        Json::Value json;
        json[ "FrameSize" ][ 0 ] = FRAME_WIDTH;
        json[ "FrameSize" ][ 1 ] = FRAME_HEIGHT;
        json[ "Clips" ][ state ] = clipJson;
        return json;
    }

    QString folderPath() const {
        return QString::fromStdString( _tempDir.string() ) + "/";
    }

    std::filesystem::path _tempDir;
};

class InvalidAppearanceCase {
public:
    InvalidAppearanceCase( std::string name, std::string state, std::string file, int southRow, bool includeFrameSize, bool includeClips ) :
        name( std::move( name ) ),
        state( std::move( state ) ),
        file( std::move( file ) ),
        southRow( southRow ),
        includeFrameSize( includeFrameSize ),
        includeClips( includeClips ) {
    }

    std::string name;
    std::string state;
    std::string file;
    int southRow;
    bool includeFrameSize;
    bool includeClips;
};

class AppearanceFactoryInvalidTest : public AppearanceFactoryTest, public ::testing::WithParamInterface<InvalidAppearanceCase> {};

} // namespace

TEST_F( AppearanceFactoryTest, CreateAppearance_CutsFramesFromRowAndColumn ) {
    const Engine::AppearanceModel model = Engine::AppearanceFactory::createAppearance( appearance( "walk", clip( 1, 2, 1 ) ), folderPath() );

    const Engine::AnimationModel& animation = model.animation( Engine::AppearanceStateEnum::WALK, Engine::AppearanceDirectionEnum::SOUTH );

    ASSERT_FALSE( animation.isNull() );
    EXPECT_TRUE( animation.isAnimated() );
    EXPECT_EQ( animation.frameDurationMs(), 100 );
    EXPECT_EQ( animation.frameAt( 0 ).size(), QSize( FRAME_WIDTH, FRAME_HEIGHT ) );
    EXPECT_EQ( animation.frameAt( 0 ).pixel( 0, 0 ), cellColor( 1, 1 ) );
    EXPECT_EQ( animation.frameAt( 100 ).pixel( 0, 0 ), cellColor( 2, 1 ) );
}

TEST_F( AppearanceFactoryTest, CreateAppearance_StrideSkipsGapBetweenFrames ) {
    writeSheet( "gap.png", 2 );

    Json::Value clipJson = clip( 1, 1, 0 );
    clipJson[ "File" ] = "gap.png";
    clipJson[ "Stride" ][ 0 ] = FRAME_WIDTH + 2;
    clipJson[ "Stride" ][ 1 ] = FRAME_HEIGHT;

    const Engine::AppearanceModel model = Engine::AppearanceFactory::createAppearance( appearance( "idle", clipJson ), folderPath() );

    const Engine::AnimationModel& animation = model.animation( Engine::AppearanceStateEnum::IDLE, Engine::AppearanceDirectionEnum::SOUTH );

    ASSERT_FALSE( animation.isNull() );
    EXPECT_EQ( animation.firstFrame().pixel( 0, 0 ), cellColor( 1, 0 ) );
    EXPECT_EQ( animation.firstFrame().pixel( FRAME_WIDTH - 1, 0 ), cellColor( 1, 0 ) );
}

TEST_F( AppearanceFactoryTest, CreateAppearance_ClipFrameSizeOverridesDefault ) {
    Json::Value clipJson = clip( 0, 1, 0 );
    clipJson[ "FrameSize" ][ 0 ] = 2;
    clipJson[ "FrameSize" ][ 1 ] = 1;

    const Engine::AppearanceModel model = Engine::AppearanceFactory::createAppearance( appearance( "idle", clipJson ), folderPath() );

    EXPECT_EQ( model.animation( Engine::AppearanceStateEnum::IDLE, Engine::AppearanceDirectionEnum::SOUTH ).firstFrame().size(), QSize( 2, 1 ) );
}

TEST_P( AppearanceFactoryInvalidTest, CreateAppearance_InvalidInput_ReturnsNullAppearance ) {
    Json::Value clipJson = clip( 0, 1, GetParam().southRow );
    clipJson[ "File" ] = GetParam().file;

    Json::Value json = appearance( GetParam().state, clipJson );
    if ( !GetParam().includeFrameSize ) {
        json.removeMember( "FrameSize" );
    }
    if ( !GetParam().includeClips ) {
        json.removeMember( "Clips" );
    }

    EXPECT_TRUE( Engine::AppearanceFactory::createAppearance( json, folderPath() ).isNull() );
}

INSTANTIATE_TEST_SUITE_P(
    Inputs,
    AppearanceFactoryInvalidTest,
    ::testing::Values(
        InvalidAppearanceCase( "MissingSheet_SkipsClip", "idle", "missing.png", 0, true, true ),
        InvalidAppearanceCase( "UnknownState_IsSkipped", "fly", "sheet.png", 0, true, true ),
        InvalidAppearanceCase( "FramesOutsideSheet_SkipsDirection", "idle", "sheet.png", ROWS, true, true ),
        InvalidAppearanceCase( "NegativeRow_SkipsDirection", "idle", "sheet.png", -1, true, true ),
        InvalidAppearanceCase( "MissingFrameSize_SkipsClip", "idle", "sheet.png", 0, false, true ),
        InvalidAppearanceCase( "MissingClips_ReturnsNull", "idle", "sheet.png", 0, true, false ) ),
    []( const ::testing::TestParamInfo<InvalidAppearanceCase>& info ) { return info.param.name; } );

TEST_F( AppearanceFactoryTest, CreateAppearance_UnknownDirection_IsSkipped ) {
    Json::Value clipJson = clip( 0, 1, 0 );
    clipJson[ "Rows" ][ "up" ] = 1;

    const Engine::AppearanceModel model = Engine::AppearanceFactory::createAppearance( appearance( "idle", clipJson ), folderPath() );

    EXPECT_FALSE( model.animation( Engine::AppearanceStateEnum::IDLE, Engine::AppearanceDirectionEnum::SOUTH ).isNull() );
    EXPECT_TRUE( model.animation( Engine::AppearanceStateEnum::IDLE, Engine::AppearanceDirectionEnum::UNKNOWN ).isNull() );
}

TEST_F( AppearanceFactoryTest, Animation_MissingState_FallsBackToIdle ) {
    const Engine::AppearanceModel model = Engine::AppearanceFactory::createAppearance( appearance( "idle", clip( 0, 1, 0 ) ), folderPath() );

    EXPECT_FALSE( model.animation( Engine::AppearanceStateEnum::ATTACK, Engine::AppearanceDirectionEnum::SOUTH ).isNull() );
}

TEST_F( AppearanceFactoryTest, Animation_MissingStateAndIdle_ReturnsNull ) {
    const Engine::AppearanceModel model = Engine::AppearanceFactory::createAppearance( appearance( "walk", clip( 0, 1, 0 ) ), folderPath() );

    EXPECT_TRUE( model.animation( Engine::AppearanceStateEnum::ATTACK, Engine::AppearanceDirectionEnum::SOUTH ).isNull() );
}

TEST( AppearanceDirectionHelperTest, FromDelta_PicksHorizontalAxisForDiagonals ) {
    using Engine::AppearanceDirectionEnum;
    using Engine::AppearanceDirectionHelper;

    EXPECT_EQ( AppearanceDirectionHelper::fromDelta( 1, 1, AppearanceDirectionEnum::NORTH ), AppearanceDirectionEnum::EAST );
    EXPECT_EQ( AppearanceDirectionHelper::fromDelta( -1, -1, AppearanceDirectionEnum::NORTH ), AppearanceDirectionEnum::WEST );
    EXPECT_EQ( AppearanceDirectionHelper::fromDelta( 0, 1, AppearanceDirectionEnum::NORTH ), AppearanceDirectionEnum::SOUTH );
    EXPECT_EQ( AppearanceDirectionHelper::fromDelta( 0, -1, AppearanceDirectionEnum::SOUTH ), AppearanceDirectionEnum::NORTH );
    EXPECT_EQ( AppearanceDirectionHelper::fromDelta( 0, 0, AppearanceDirectionEnum::WEST ), AppearanceDirectionEnum::WEST );
}
