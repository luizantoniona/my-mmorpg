#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <QImage>

#include <MMORPGEngine/Data/DataFactory.h>

namespace {

class TexturePathCase {
public:
    TexturePathCase( std::string name, bool isAnimated, std::vector<std::string> existingExtensions, std::string expectedExtension ) :
        name( std::move( name ) ),
        isAnimated( isAnimated ),
        existingExtensions( std::move( existingExtensions ) ),
        expectedExtension( std::move( expectedExtension ) ) {
    }

    std::string name;
    bool isAnimated;
    std::vector<std::string> existingExtensions;
    std::string expectedExtension;
};

class DataFactoryTexturePathTest : public ::testing::TestWithParam<TexturePathCase> {
protected:
    void SetUp() override {
        _tempDir = std::filesystem::temp_directory_path() / "mmorpg_engine_test_datafactory";
        std::filesystem::create_directories( _tempDir );
    }

    void TearDown() override {
        std::filesystem::remove_all( _tempDir );
    }

    std::filesystem::path _tempDir;
};

} // namespace

TEST( DataFactoryLoadAnimationTest, SingleFrameWebp_FlaggedAsAnimated_LoadsOneFrameAndTerminates ) {
    const std::filesystem::path path = std::filesystem::temp_directory_path() / "mmorpg_engine_test_single_frame.webp";

    QImage image( 8, 8, QImage::Format_ARGB32 );
    image.fill( Qt::green );
    ASSERT_TRUE( image.save( QString::fromStdString( path.string() ) ) );

    const Engine::AnimationModel animation = Engine::DataFactory::loadAnimation( QString::fromStdString( path.string() ), true, 100 );

    std::filesystem::remove( path );

    EXPECT_FALSE( animation.isNull() );
    EXPECT_FALSE( animation.isAnimated() );
    EXPECT_EQ( animation.firstFrame().size(), QSize( 8, 8 ) );
}

TEST( DataFactoryLoadAnimationTest, MissingFile_FlaggedAsAnimated_ReturnsNullAnimation ) {
    const Engine::AnimationModel animation = Engine::DataFactory::loadAnimation( "mmorpg_engine_test_definitely_missing.webp", true, 100 );

    EXPECT_TRUE( animation.isNull() );
}

TEST_P( DataFactoryTexturePathTest, ResolveTexturePath_PicksExtensionByAnimatedFlagAndExistingFile ) {
    for ( const std::string& extension : GetParam().existingExtensions ) {
        std::ofstream( _tempDir / ( "texture" + extension ) ) << "x";
    }

    const QString basePath = QString::fromStdString( ( _tempDir / "texture" ).string() );

    const QString resolved = Engine::DataFactory::resolveTexturePath( basePath, GetParam().isAnimated );

    EXPECT_EQ( resolved, basePath + QString::fromStdString( GetParam().expectedExtension ) );
}

INSTANTIATE_TEST_SUITE_P(
    Extensions,
    DataFactoryTexturePathTest,
    ::testing::Values(
        TexturePathCase( "Static_PngExists_UsesPng", false, { ".png" }, ".png" ),
        TexturePathCase( "Static_NoFile_FallsBackToPng", false, {}, ".png" ),
        TexturePathCase( "Static_OnlyGifExists_IgnoresGifAndFallsBackToPng", false, { ".gif" }, ".png" ),
        TexturePathCase( "Animated_GifExists_UsesGif", true, { ".gif" }, ".gif" ),
        TexturePathCase( "Animated_WebpExists_UsesWebp", true, { ".webp" }, ".webp" ),
        TexturePathCase( "Animated_GifAndWebpExist_PrefersGif", true, { ".gif", ".webp" }, ".gif" ),
        TexturePathCase( "Animated_OnlyPngExists_IgnoresPngAndFallsBackToGif", true, { ".png" }, ".gif" ),
        TexturePathCase( "Animated_NoFile_FallsBackToGif", true, {}, ".gif" ) ),
    []( const ::testing::TestParamInfo<TexturePathCase>& info ) { return info.param.name; } );
