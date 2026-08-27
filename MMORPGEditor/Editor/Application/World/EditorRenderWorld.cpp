#include "EditorRenderWorld.h"

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/World/WorldFactory.h>

EditorRenderWorld::EditorRenderWorld( QObject* parent ) :
    Engine::RenderWorld( parent ),
    _world( nullptr ) {
}

Engine::WorldModel* EditorRenderWorld::world() const {
    return _world;
}

void EditorRenderWorld::setWorld( Engine::WorldModel* world ) {
    _world = world;
}

const Engine::WorldObjectModel* EditorRenderWorld::object( int x, int y, int z ) const {
    if ( !_world ) {
        return nullptr;
    }

    return _world->object( x, y, z );
}

const Engine::WorldTileModel* EditorRenderWorld::tile( int x, int y, int z ) const {
    if ( !_world ) {
        return nullptr;
    }

    return _world->tile( x, y, z );
}
