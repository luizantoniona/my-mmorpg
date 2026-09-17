#include <gtest/gtest.h>

#include <MMORPGEditor/Editor/Application/World/Tile/TileSelectionControl.h>

TEST( TileSelectionControlTest, DefaultConstructor_HasNoSelection ) {
    const TileSelectionControl control;

    EXPECT_FALSE( control.hasSelection() );
}

TEST( TileSelectionControlTest, SelectTile_SetsCoordinatesAndHasSelection ) {
    TileSelectionControl control;

    control.selectTile( 3, 4, 1 );

    EXPECT_TRUE( control.hasSelection() );
    EXPECT_EQ( control.x(), 3 );
    EXPECT_EQ( control.y(), 4 );
    EXPECT_EQ( control.z(), 1 );
}

TEST( TileSelectionControlTest, SelectTile_EmitsSelectionChanged ) {
    TileSelectionControl control;

    int emitCount = 0;
    QObject::connect( &control, &TileSelectionControl::selectionChanged, [ &emitCount ]() {
        ++emitCount;
    } );

    control.selectTile( 3, 4, 1 );

    EXPECT_EQ( emitCount, 1 );
}

TEST( TileSelectionControlTest, SelectTile_SameCoordinatesTwice_DoesNotEmitAgain ) {
    TileSelectionControl control;
    control.selectTile( 3, 4, 1 );

    int emitCount = 0;
    QObject::connect( &control, &TileSelectionControl::selectionChanged, [ &emitCount ]() {
        ++emitCount;
    } );

    control.selectTile( 3, 4, 1 );

    EXPECT_EQ( emitCount, 0 );
}

TEST( TileSelectionControlTest, SelectTile_DifferentCoordinates_EmitsAgain ) {
    TileSelectionControl control;
    control.selectTile( 3, 4, 1 );

    int emitCount = 0;
    QObject::connect( &control, &TileSelectionControl::selectionChanged, [ &emitCount ]() {
        ++emitCount;
    } );

    control.selectTile( 3, 4, 2 );

    EXPECT_EQ( emitCount, 1 );
}

TEST( TileSelectionControlTest, ClearSelection_WhenSelected_ClearsAndEmits ) {
    TileSelectionControl control;
    control.selectTile( 3, 4, 1 );

    int emitCount = 0;
    QObject::connect( &control, &TileSelectionControl::selectionChanged, [ &emitCount ]() {
        ++emitCount;
    } );

    control.clearSelection();

    EXPECT_FALSE( control.hasSelection() );
    EXPECT_EQ( emitCount, 1 );
}

TEST( TileSelectionControlTest, ClearSelection_WhenAlreadyEmpty_DoesNotEmit ) {
    TileSelectionControl control;

    int emitCount = 0;
    QObject::connect( &control, &TileSelectionControl::selectionChanged, [ &emitCount ]() {
        ++emitCount;
    } );

    control.clearSelection();

    EXPECT_EQ( emitCount, 0 );
}
