#include "WorldControl.h"

#include <MMORPGEngine/World/WorldFactory.h>

namespace {
constexpr const char* DATA_PATH = "../../../Data/";
}

WorldControl::WorldControl( QObject* parent ) :
    QObject( parent ),
    _world( nullptr ) {
}

Engine::WorldModel* WorldControl::world() const {
    return _world.get();
}

QVariantList WorldControl::floors() const {
    QVariantList result;

    if ( !_world ) {
        return result;
    }

    for ( int z : _world->floors() ) {
        result.append( z );
    }

    return result;
}

QString WorldControl::worldName() const {
    if ( !_world ) {
        return "";
    }

    return _world->name();
}

int WorldControl::worldWidth() const {
    if ( !_world ) {
        return 0;
    }

    return static_cast<int>( _world->width() );
}

int WorldControl::worldHeight() const {
    if ( !_world ) {
        return 0;
    }

    return static_cast<int>( _world->height() );
}

void WorldControl::loadWorld() {
    _world = Engine::WorldFactory::createWorld( DATA_PATH );

    emit worldChanged();
}

bool WorldControl::saveWorld() {
    if ( !_world ) {
        return false;
    }

    Engine::WorldFactory::saveWorld( DATA_PATH, *_world );

    return true;
}

bool WorldControl::addFloor( bool above ) {
    if ( !_world ) {
        return false;
    }

    if ( !Engine::WorldFactory::addFloor( DATA_PATH, *_world, above ) ) {
        return false;
    }

    loadWorld();

    return true;
}

bool WorldControl::removeFloor( int z ) {
    if ( !_world ) {
        return false;
    }

    if ( !Engine::WorldFactory::removeFloor( DATA_PATH, z ) ) {
        return false;
    }

    loadWorld();

    return true;
}

void WorldControl::paintTile( int x, int y, int z, int tileType ) {
    if ( !_world ) {
        return;
    }

    _world->setTile( x, y, z, static_cast<uint32_t>( tileType ) );
}

void WorldControl::paintObject( int x, int y, int z, int objectType ) {
    if ( !_world ) {
        return;
    }

    _world->setObject( x, y, z, static_cast<uint32_t>( objectType ) );
}
