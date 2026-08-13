#include "WorldPageControl.h"

#include <MMORPGEngine/World/WorldFactory.h>

namespace {
constexpr const char* DATA_PATH = "../../../Data/";
}

WorldPageControl::WorldPageControl( QObject* parent ) :
    QObject( parent ),
    _world( nullptr ) {
}

QString WorldPageControl::worldName() const {
    if ( !_world ) {
        return "";
    }

    return QString::fromStdString( _world->name() );
}

int WorldPageControl::worldWidth() const {
    if ( !_world ) {
        return 0;
    }

    return static_cast<int>( _world->width() );
}

int WorldPageControl::worldHeight() const {
    if ( !_world ) {
        return 0;
    }

    return static_cast<int>( _world->height() );
}

void WorldPageControl::loadWorld() {
    _world = Engine::WorldFactory::createWorld( DATA_PATH );

    emit worldChanged();
}
