#include <gtest/gtest.h>

#include <MMORPGEngine/Renderer/Camera/Camera.h>
#include <MMORPGEngine/World/WorldConstants.h>

TEST( CameraTest, DefaultConstructor_IsOriginWithZoomOne ) {
    const Engine::Camera camera;

    EXPECT_EQ( camera.position(), QPointF( 0.0, 0.0 ) );
    EXPECT_DOUBLE_EQ( camera.zoom(), 1.0 );
}

TEST( CameraTest, SetZoom_NonPositiveValue_IsIgnored ) {
    Engine::Camera camera;

    camera.setZoom( 0.0 );
    EXPECT_DOUBLE_EQ( camera.zoom(), 1.0 );

    camera.setZoom( -2.0 );
    EXPECT_DOUBLE_EQ( camera.zoom(), 1.0 );

    camera.setZoom( 2.0 );
    EXPECT_DOUBLE_EQ( camera.zoom(), 2.0 );
}

TEST( CameraTest, CenterOnTile_UsesTileCenter ) {
    Engine::Camera camera;

    camera.centerOnTile( 1, 2 );

    const double tileSize = Engine::WorldConstants::TILE_SIZE;
    EXPECT_EQ( camera.position(), QPointF( tileSize + tileSize / 2.0, 2 * tileSize + tileSize / 2.0 ) );
}

TEST( CameraTest, MoveByTiles_OffsetsPositionByTileSize ) {
    Engine::Camera camera;
    camera.centerOnTile( 0, 0 );

    camera.moveByTiles( 2, -1 );

    const double tileSize = Engine::WorldConstants::TILE_SIZE;
    EXPECT_EQ( camera.position(), QPointF( tileSize / 2.0 + 2 * tileSize, tileSize / 2.0 - tileSize ) );
}

TEST( CameraTest, WorldToScreen_ThenScreenToWorld_RoundTrips ) {
    Engine::Camera camera;
    camera.setViewportSize( QSizeF( 800.0, 600.0 ) );
    camera.setZoom( 2.0 );
    camera.setPosition( QPointF( 100.0, 50.0 ) );

    const QPointF worldPoint( 130.0, 70.0 );
    const QPointF screenPoint = camera.worldToScreen( worldPoint );
    const QPointF roundTripped = camera.screenToWorld( screenPoint );

    EXPECT_DOUBLE_EQ( roundTripped.x(), worldPoint.x() );
    EXPECT_DOUBLE_EQ( roundTripped.y(), worldPoint.y() );
}

TEST( CameraTest, VisibleRect_CenteredOnPosition ) {
    Engine::Camera camera;
    camera.setViewportSize( QSizeF( 800.0, 600.0 ) );
    camera.setPosition( QPointF( 0.0, 0.0 ) );

    const QRectF visible = camera.visibleRect();

    EXPECT_EQ( visible, QRectF( -400.0, -300.0, 800.0, 600.0 ) );
}

TEST( CameraTest, ClampPosition_WorldSmallerThanViewport_CentersOnWorld ) {
    Engine::Camera camera;
    camera.setViewportSize( QSizeF( 800.0, 600.0 ) );
    camera.setWorldSize( QSizeF( 200.0, 100.0 ) );

    camera.setPosition( QPointF( 1000.0, 1000.0 ) );

    EXPECT_EQ( camera.position(), QPointF( 100.0, 50.0 ) );
}

TEST( CameraTest, ClampPosition_WorldLargerThanViewport_ClampsToBounds ) {
    Engine::Camera camera;
    camera.setViewportSize( QSizeF( 800.0, 600.0 ) );
    camera.setWorldSize( QSizeF( 2000.0, 2000.0 ) );

    camera.setPosition( QPointF( -500.0, 5000.0 ) );

    EXPECT_EQ( camera.position(), QPointF( 400.0, 1700.0 ) );
}
