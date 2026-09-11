#include "TileSelectionControl.h"

TileSelectionControl::TileSelectionControl( QObject* parent ) :
    QObject( parent ),
    _hasSelection( false ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ) {
}

bool TileSelectionControl::hasSelection() const {
    return _hasSelection;
}

int TileSelectionControl::x() const {
    return _x;
}

int TileSelectionControl::y() const {
    return _y;
}

int TileSelectionControl::z() const {
    return _z;
}

void TileSelectionControl::selectTile( int x, int y, int z ) {
    if ( _hasSelection && _x == x && _y == y && _z == z ) {
        return;
    }

    _hasSelection = true;

    _x = x;
    _y = y;
    _z = z;

    emit selectionChanged();
}

void TileSelectionControl::clearSelection() {
    if ( !_hasSelection ) {
        return;
    }

    _hasSelection = false;

    emit selectionChanged();
}
