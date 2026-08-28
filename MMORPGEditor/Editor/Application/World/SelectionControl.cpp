#include "SelectionControl.h"

SelectionControl::SelectionControl( QObject* parent ) :
    QObject( parent ),
    _hasSelection( false ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ) {
}

bool SelectionControl::hasSelection() const {
    return _hasSelection;
}

int SelectionControl::x() const {
    return _x;
}

int SelectionControl::y() const {
    return _y;
}

int SelectionControl::z() const {
    return _z;
}

void SelectionControl::selectTile( int x, int y, int z ) {
    if ( _hasSelection && _x == x && _y == y && _z == z ) {
        return;
    }

    _hasSelection = true;

    _x = x;
    _y = y;
    _z = z;

    emit selectionChanged();
}

void SelectionControl::clearSelection() {
    if ( !_hasSelection ) {
        return;
    }

    _hasSelection = false;

    emit selectionChanged();
}
