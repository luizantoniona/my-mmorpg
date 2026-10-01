#include "WorldControl.h"

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/Creature/CreatureSpawnAreaModel.h>
#include <MMORPGEngine/Data/Creature/CreatureSpawnEntryModel.h>
#include <MMORPGEngine/Data/Creature/CreatureTypeCatalog.h>
#include <MMORPGEngine/Data/Creature/CreatureTypeModel.h>
#include <MMORPGEngine/Data/DataManager.h>
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

void WorldControl::paintSpawnArea( int x, int y, int z, int width, int height, const QVariantList& creatures ) {
    if ( !_world ) {
        return;
    }

    Engine::CreatureSpawnAreaModel area;
    area.setX( x );
    area.setY( y );
    area.setWidth( static_cast<uint32_t>( width ) );
    area.setHeight( static_cast<uint32_t>( height ) );

    std::vector<Engine::CreatureSpawnEntryModel> entries;
    for ( const QVariant& creatureVariant : creatures ) {
        const QVariantMap creatureMap = creatureVariant.toMap();

        Engine::CreatureSpawnEntryModel entry;
        entry.setType( creatureMap.value( "type" ).toUInt() );
        entry.setQuantity( creatureMap.value( "quantity" ).toUInt() );

        entries.push_back( entry );
    }
    area.setCreatures( entries );

    _world->addSpawnArea( z, area );
}

QVariantList WorldControl::spawnAreas( int z ) const {
    QVariantList result;

    if ( !_world ) {
        return result;
    }

    for ( const Engine::CreatureSpawnAreaModel& area : _world->spawnAreas( z ) ) {
        QVariantMap areaMap;
        areaMap[ "x" ] = area.x();
        areaMap[ "y" ] = area.y();
        areaMap[ "width" ] = area.width();
        areaMap[ "height" ] = area.height();

        result.append( areaMap );
    }

    return result;
}

QVariantMap WorldControl::spawnAreaAt( int x, int y, int z ) const {
    QVariantMap result;

    if ( !_world ) {
        return result;
    }

    const Engine::CreatureSpawnAreaModel* area = _world->spawnAreaAt( x, y, z );
    if ( !area ) {
        return result;
    }

    result[ "x" ] = area->x();
    result[ "y" ] = area->y();
    result[ "width" ] = area->width();
    result[ "height" ] = area->height();

    const Engine::CreatureTypeCatalog& catalog = Engine::Singleton<Engine::DataManager>::instance().creatureTypeCatalog();

    QVariantList creatures;
    for ( const Engine::CreatureSpawnEntryModel& entry : area->creatures() ) {
        const Engine::CreatureTypeModel* creatureType = catalog.creatureType( entry.type() );

        QVariantMap entryMap;
        entryMap[ "type" ] = entry.type();
        entryMap[ "quantity" ] = entry.quantity();
        entryMap[ "name" ] = creatureType ? creatureType->name() : QString();

        creatures.append( entryMap );
    }
    result[ "creatures" ] = creatures;

    return result;
}

bool WorldControl::removeSpawnArea( int x, int y, int z ) {
    if ( !_world ) {
        return false;
    }

    return _world->removeSpawnArea( x, y, z );
}
