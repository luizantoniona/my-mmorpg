#include "WorldControl.h"

#include <MMORPGEngine/World/WorldFactory.h>

namespace {
constexpr const char* DATA_PATH = "../../../Data/";
}

WorldControl::WorldControl( QObject* parent ) :
    QObject( parent ),
    _world( nullptr ) {
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

Engine::WorldModel* WorldControl::world() const {
    return _world.get();
}

void WorldControl::loadWorld() {
    _world = Engine::WorldFactory::createWorld( DATA_PATH );

    emit worldChanged();
}
