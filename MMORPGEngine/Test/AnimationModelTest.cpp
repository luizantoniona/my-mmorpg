#include <gtest/gtest.h>

#include <MMORPGEngine/Data/Animation/AnimationModel.h>

namespace {

QImage makeImage( int width ) {
    QImage image( width, 1, QImage::Format_RGB32 );
    image.fill( Qt::black );
    return image;
}

} // namespace

TEST( AnimationModelTest, DefaultConstructor_IsNull ) {
    const Engine::AnimationModel animation;

    EXPECT_TRUE( animation.isNull() );
    EXPECT_FALSE( animation.isAnimated() );
    EXPECT_EQ( animation.frameDurationMs(), 100 );
}

TEST( AnimationModelTest, NonPositiveFrameDuration_FallsBackToDefault ) {
    const Engine::AnimationModel animation( { makeImage( 1 ) }, 0 );

    EXPECT_EQ( animation.frameDurationMs(), 100 );
}

TEST( AnimationModelTest, SingleFrame_IsNotAnimated ) {
    const Engine::AnimationModel animation( { makeImage( 1 ) }, 100 );

    EXPECT_FALSE( animation.isNull() );
    EXPECT_FALSE( animation.isAnimated() );
    EXPECT_EQ( animation.frameAt( 999 ).width(), 1 );
}

TEST( AnimationModelTest, MultipleFrames_IsAnimated ) {
    const Engine::AnimationModel animation( { makeImage( 1 ), makeImage( 2 ) }, 100 );

    EXPECT_TRUE( animation.isAnimated() );
}

TEST( AnimationModelTest, FrameAt_CyclesThroughFramesByElapsedTime ) {
    const Engine::AnimationModel animation( { makeImage( 1 ), makeImage( 2 ), makeImage( 3 ) }, 100 );

    EXPECT_EQ( animation.frameAt( 0 ).width(), 1 );
    EXPECT_EQ( animation.frameAt( 150 ).width(), 2 );
    EXPECT_EQ( animation.frameAt( 250 ).width(), 3 );
    EXPECT_EQ( animation.frameAt( 300 ).width(), 1 );
}

TEST( AnimationModelTest, FirstFrame_ReturnsFirstOfList ) {
    const Engine::AnimationModel animation( { makeImage( 5 ), makeImage( 6 ) }, 100 );

    EXPECT_EQ( animation.firstFrame().width(), 5 );
}
