#include "ObjectSelectionControl.h"

ObjectSelectionControl::ObjectSelectionControl( QObject* parent ) :
    QObject( parent ),
    _hasSelection( false ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ) {
}

bool ObjectSelectionControl::hasSelection() const {
    return _hasSelection;
}

int ObjectSelectionControl::x() const {
    return _x;
}

int ObjectSelectionControl::y() const {
    return _y;
}

int ObjectSelectionControl::z() const {
    return _z;
}

void ObjectSelectionControl::selectObject( int x, int y, int z ) {
    if ( _hasSelection && _x == x && _y == y && _z == z ) {
        return;
    }

    _hasSelection = true;

    _x = x;
    _y = y;
    _z = z;

    emit selectionChanged();
}

void ObjectSelectionControl::clearSelection() {
    if ( !_hasSelection ) {
        return;
    }

    _hasSelection = false;

    emit selectionChanged();
}
