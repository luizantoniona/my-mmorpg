#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include <QImage>

#include <MMORPGEngine/Data/Character/CharacterAppearanceFactory.h>

namespace {

constexpr int FRAME_WIDTH = 4;
constexpr int FRAME_HEIGHT = 2;
constexpr int IDLE_FRAMES = 3;
constexpr int FRAME_DURATION_MS = 100;

Engine::CharacterAppearanceModel bodyAppearance( const QString& idBody ) {
    Engine::CharacterAppearanceModel appearance;
    appearance.setId( Engine::CharacterAppearanceFeatureEnum::BODY, idBody );

    return appearance;
}

class CharacterAppearanceFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_characterappearancefactory";
        std::filesystem::create_directories( _tempDir );

        writeBodySheet( "body_idle.png" );
        writeHeadSheet( "head_idle.png" );

        _catalog.setFrameSize( QSize( FRAME_WIDTH, FRAME_HEIGHT ) );
        _catalog.setRow( Engine::AppearanceDirectionEnum::NORTH, 0 );
        _catalog.setRow( Engine::AppearanceDirectionEnum::WEST, 1 );
        _catalog.setRow( Engine::AppearanceDirectionEnum::SOUTH, 2 );
        _catalog.setRow( Engine::AppearanceDirectionEnum::EAST, 3 );
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    static QRgb bodyColor( int column, int row ) {
        return qRgba( 10 + column * 40, 10 + row * 40, 100, 255 );
    }

    static QRgb headColor() {
        return qRgba( 250, 250, 250, 255 );
    }

    QString path( const std::string& file ) const {
        return QString::fromStdString( ( _tempDir / file ).string() );
    }

    void writeBodySheet( const std::string& file ) const {
        QImage sheet( IDLE_FRAMES * FRAME_WIDTH, 4 * FRAME_HEIGHT, QImage::Format_ARGB32 );

        for ( int y = 0; y < sheet.height(); ++y ) {
            for ( int x = 0; x < sheet.width(); ++x ) {
                sheet.setPixel( x, y, bodyColor( x / FRAME_WIDTH, y / FRAME_HEIGHT ) );
            }
        }

        sheet.save( path( file ) );
    }

    void writeHeadSheet( const std::string& file ) const {
        QImage sheet( IDLE_FRAMES * FRAME_WIDTH, 4 * FRAME_HEIGHT, QImage::Format_ARGB32 );
        sheet.fill( Qt::transparent );

        for ( int row = 0; row < 4; ++row ) {
            for ( int column = 0; column < IDLE_FRAMES; ++column ) {
                sheet.setPixel( column * FRAME_WIDTH, row * FRAME_HEIGHT, headColor() );
            }
        }

        sheet.save( path( file ) );
    }

    void addBody( const std::string& id, bool withHead ) {
        Engine::CharacterBodyModel body;
        body.setId( QString::fromStdString( id ) );
        body.sprite().setFolder( QString::fromStdString( _tempDir.string() ) + "/" );
        body.sprite().setFile( Engine::CharacterLayerEnum::BODY, Engine::AppearanceStateEnum::IDLE, "body_idle.png" );

        if ( withHead ) {
            body.sprite().setFile( Engine::CharacterLayerEnum::HEAD, Engine::AppearanceStateEnum::IDLE, "head_idle.png" );
        }

        body.setClip( Engine::AppearanceStateEnum::IDLE, Engine::CharacterClipModel( IDLE_FRAMES, FRAME_DURATION_MS ) );

        _catalog.addBody( body );
    }

    std::filesystem::path _tempDir;
    Engine::CharacterCatalog _catalog;
};

TEST_F( CharacterAppearanceFactoryTest, CreateAppearance_UnknownBody_ReturnsNullAppearance ) {
    addBody( "HERO", true );

    const Engine::AppearanceModel appearance = Engine::CharacterAppearanceFactory::createAppearance( _catalog, bodyAppearance( "NOBODY" ) );

    EXPECT_TRUE( appearance.isNull() );
}

TEST_F( CharacterAppearanceFactoryTest, CreateAppearance_EachDirection_UsesItsRowOfTheSheet ) {
    addBody( "HERO", false );

    const Engine::AppearanceModel appearance = Engine::CharacterAppearanceFactory::createAppearance( _catalog, bodyAppearance( "HERO" ) );

    EXPECT_EQ( appearance.animation( Engine::AppearanceStateEnum::IDLE, Engine::AppearanceDirectionEnum::NORTH ).frameAt( 0 ).pixel( 1, 1 ), bodyColor( 0, 0 ) );
    EXPECT_EQ( appearance.animation( Engine::AppearanceStateEnum::IDLE, Engine::AppearanceDirectionEnum::WEST ).frameAt( 0 ).pixel( 1, 1 ), bodyColor( 0, 1 ) );
    EXPECT_EQ( appearance.animation( Engine::AppearanceStateEnum::IDLE, Engine::AppearanceDirectionEnum::SOUTH ).frameAt( 0 ).pixel( 1, 1 ), bodyColor( 0, 2 ) );
    EXPECT_EQ( appearance.animation( Engine::AppearanceStateEnum::IDLE, Engine::AppearanceDirectionEnum::EAST ).frameAt( 0 ).pixel( 1, 1 ), bodyColor( 0, 3 ) );
}

TEST_F( CharacterAppearanceFactoryTest, CreateAppearance_AdvancingTime_WalksTheColumns ) {
    addBody( "HERO", false );

    const Engine::AppearanceModel appearance = Engine::CharacterAppearanceFactory::createAppearance( _catalog, bodyAppearance( "HERO" ) );
    const Engine::AnimationModel& animation = appearance.animation( Engine::AppearanceStateEnum::IDLE, Engine::AppearanceDirectionEnum::SOUTH );

    EXPECT_TRUE( animation.isAnimated() );
    EXPECT_EQ( animation.frameDurationMs(), FRAME_DURATION_MS );
    EXPECT_EQ( animation.frameAt( FRAME_DURATION_MS ).pixel( 1, 1 ), bodyColor( 1, 2 ) );
    EXPECT_EQ( animation.frameAt( 2 * FRAME_DURATION_MS ).pixel( 1, 1 ), bodyColor( 2, 2 ) );
}

TEST_F( CharacterAppearanceFactoryTest, CreateAppearance_HeadLayer_IsDrawnOverTheBody ) {
    addBody( "HERO", true );

    const Engine::AppearanceModel appearance = Engine::CharacterAppearanceFactory::createAppearance( _catalog, bodyAppearance( "HERO" ) );
    const QImage frame = appearance.animation( Engine::AppearanceStateEnum::IDLE, Engine::AppearanceDirectionEnum::SOUTH ).frameAt( 0 );

    EXPECT_EQ( frame.pixel( 0, 0 ), headColor() );
    EXPECT_EQ( frame.pixel( 1, 1 ), bodyColor( 0, 2 ) );
}

TEST_F( CharacterAppearanceFactoryTest, CreateAppearance_StateWithoutClip_FallsBackToIdle ) {
    addBody( "HERO", false );

    const Engine::AppearanceModel appearance = Engine::CharacterAppearanceFactory::createAppearance( _catalog, bodyAppearance( "HERO" ) );

    EXPECT_FALSE( appearance.animation( Engine::AppearanceStateEnum::IDLE, Engine::AppearanceDirectionEnum::SOUTH ).isNull() );
    EXPECT_EQ( appearance.animation( Engine::AppearanceStateEnum::ATTACK, Engine::AppearanceDirectionEnum::SOUTH ).frameAt( 0 ).pixel( 1, 1 ), bodyColor( 0, 2 ) );
}

} // namespace
