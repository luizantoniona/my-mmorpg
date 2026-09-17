#include <gtest/gtest.h>

#include <MMORPGEditor/Editor/Application/World/Object/ObjectSelectionControl.h>

TEST( ObjectSelectionControlTest, DefaultConstructor_HasNoSelection ) {
    const ObjectSelectionControl control;

    EXPECT_FALSE( control.hasSelection() );
}

TEST( ObjectSelectionControlTest, SelectObject_SetsCoordinatesAndHasSelection ) {
    ObjectSelectionControl control;

    control.selectObject( 3, 4, 1 );

    EXPECT_TRUE( control.hasSelection() );
    EXPECT_EQ( control.x(), 3 );
    EXPECT_EQ( control.y(), 4 );
    EXPECT_EQ( control.z(), 1 );
}

TEST( ObjectSelectionControlTest, SelectObject_EmitsSelectionChanged ) {
    ObjectSelectionControl control;

    int emitCount = 0;
    QObject::connect( &control, &ObjectSelectionControl::selectionChanged, [ &emitCount ]() {
        ++emitCount;
    } );

    control.selectObject( 3, 4, 1 );

    EXPECT_EQ( emitCount, 1 );
}

TEST( ObjectSelectionControlTest, SelectObject_SameCoordinatesTwice_DoesNotEmitAgain ) {
    ObjectSelectionControl control;
    control.selectObject( 3, 4, 1 );

    int emitCount = 0;
    QObject::connect( &control, &ObjectSelectionControl::selectionChanged, [ &emitCount ]() {
        ++emitCount;
    } );

    control.selectObject( 3, 4, 1 );

    EXPECT_EQ( emitCount, 0 );
}

TEST( ObjectSelectionControlTest, ClearSelection_WhenSelected_ClearsAndEmits ) {
    ObjectSelectionControl control;
    control.selectObject( 3, 4, 1 );

    int emitCount = 0;
    QObject::connect( &control, &ObjectSelectionControl::selectionChanged, [ &emitCount ]() {
        ++emitCount;
    } );

    control.clearSelection();

    EXPECT_FALSE( control.hasSelection() );
    EXPECT_EQ( emitCount, 1 );
}

TEST( ObjectSelectionControlTest, ClearSelection_WhenAlreadyEmpty_DoesNotEmit ) {
    ObjectSelectionControl control;

    int emitCount = 0;
    QObject::connect( &control, &ObjectSelectionControl::selectionChanged, [ &emitCount ]() {
        ++emitCount;
    } );

    control.clearSelection();

    EXPECT_EQ( emitCount, 0 );
}
