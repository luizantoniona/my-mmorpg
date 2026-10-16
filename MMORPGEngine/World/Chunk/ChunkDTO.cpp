#include "ChunkDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>
#include <MMORPGEngine/World/WorldConstants.h>

namespace {

constexpr int CELLS_PER_CHUNK = Engine::WorldConstants::CHUNK_SIZE * Engine::WorldConstants::CHUNK_SIZE;

Json::Value cellsToJson( const std::vector<Engine::ChunkDTO::Cell>& cells ) {
    Json::Value cellsJson( Json::arrayValue );

    for ( const Engine::ChunkDTO::Cell& cell : cells ) {
        Json::Value cellJson;
        cellJson[ "X" ] = cell.x;
        cellJson[ "Y" ] = cell.y;
        cellJson[ "Type" ] = cell.type;

        cellsJson.append( cellJson );
    }

    return cellsJson;
}

std::vector<Engine::ChunkDTO::Cell> cellsFromJson( const Json::Value& cellsJson ) {
    std::vector<Engine::ChunkDTO::Cell> cells;

    if ( !cellsJson.isArray() ) {
        return cells;
    }

    for ( const Json::Value& cellJson : cellsJson ) {
        if ( !cellJson.isObject() || !cellJson[ "X" ].isInt() || !cellJson[ "Y" ].isInt() || !cellJson[ "Type" ].isUInt() ) {
            continue;
        }

        cells.emplace_back( cellJson[ "X" ].asInt(), cellJson[ "Y" ].asInt(), cellJson[ "Type" ].asUInt() );
    }

    return cells;
}

} // namespace

namespace Engine {

ChunkDTO::Cell::Cell( int x, int y, uint32_t type ) :
    x( x ),
    y( y ),
    type( type ) {
}

ChunkDTO::Floor::Floor() :
    z( 0 ) {
}

ChunkDTO::ChunkDTO() :
    _chunkX( 0 ),
    _chunkY( 0 ) {
}

ChunkDTO::~ChunkDTO() = default;

ChunkDTO ChunkDTO::fromModel( const WorldModel& world, int chunkX, int chunkY ) {
    ChunkDTO dto;
    dto._chunkX = chunkX;
    dto._chunkY = chunkY;

    const ChunkModel* chunk = world.chunk( chunkX, chunkY );
    if ( !chunk ) {
        return dto;
    }

    for ( int z : world.floors() ) {
        Floor floor;
        floor.z = z;
        floor.tiles.assign( CELLS_PER_CHUNK, 0 );

        bool hasContent = false;

        for ( int y = 0; y < WorldConstants::CHUNK_SIZE; ++y ) {
            for ( int x = 0; x < WorldConstants::CHUNK_SIZE; ++x ) {
                const WorldTileModel* tile = chunk->tile( x, y, z );
                if ( tile && tile->tileType() != 0 ) {
                    floor.tiles[ y * WorldConstants::CHUNK_SIZE + x ] = tile->tileType();
                    hasContent = true;
                }

                if ( tile && tile->overlayType() != 0 ) {
                    floor.overlays.emplace_back( x, y, tile->overlayType() );
                    hasContent = true;
                }

                const WorldObjectModel* object = chunk->object( x, y, z );
                if ( object && object->objectType() != 0 ) {
                    floor.objects.emplace_back( x, y, object->objectType() );
                    hasContent = true;
                }
            }
        }

        if ( hasContent ) {
            dto._floors.push_back( floor );
        }
    }

    return dto;
}

ChunkDTO ChunkDTO::fromJson( const Json::Value& json ) {
    ChunkDTO dto;

    if ( json.isMember( "chunkX" ) && json[ "chunkX" ].isInt() ) {
        dto._chunkX = json[ "chunkX" ].asInt();
    }

    if ( json.isMember( "chunkY" ) && json[ "chunkY" ].isInt() ) {
        dto._chunkY = json[ "chunkY" ].asInt();
    }

    if ( !json.isMember( "floors" ) || !json[ "floors" ].isArray() ) {
        return dto;
    }

    for ( const Json::Value& floorJson : json[ "floors" ] ) {
        if ( !floorJson.isObject() || !floorJson[ "z" ].isInt() ) {
            continue;
        }

        Floor floor;
        floor.z = floorJson[ "z" ].asInt();

        floor.tiles.assign( CELLS_PER_CHUNK, 0 );
        const Json::Value& tilesJson = floorJson[ "tiles" ];
        if ( tilesJson.isArray() && static_cast<int>( tilesJson.size() ) == CELLS_PER_CHUNK ) {
            for ( int i = 0; i < CELLS_PER_CHUNK; ++i ) {
                floor.tiles[ i ] = tilesJson[ i ].asUInt();
            }
        }

        floor.overlays = cellsFromJson( floorJson[ "overlays" ] );
        floor.objects = cellsFromJson( floorJson[ "objects" ] );

        dto._floors.push_back( floor );
    }

    return dto;
}

Json::Value ChunkDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::CHUNK );
    json[ "chunkX" ] = _chunkX;
    json[ "chunkY" ] = _chunkY;

    Json::Value floorsJson( Json::arrayValue );
    for ( const Floor& floor : _floors ) {
        Json::Value floorJson;
        floorJson[ "z" ] = floor.z;

        Json::Value tilesJson( Json::arrayValue );
        for ( uint32_t tile : floor.tiles ) {
            tilesJson.append( tile );
        }
        floorJson[ "tiles" ] = tilesJson;
        floorJson[ "overlays" ] = cellsToJson( floor.overlays );
        floorJson[ "objects" ] = cellsToJson( floor.objects );

        floorsJson.append( floorJson );
    }
    json[ "floors" ] = floorsJson;

    return json;
}

void ChunkDTO::applyTo( WorldModel& world ) const {
    const int originX = _chunkX * WorldConstants::CHUNK_SIZE;
    const int originY = _chunkY * WorldConstants::CHUNK_SIZE;

    for ( const Floor& floor : _floors ) {
        world.addFloor( floor.z );

        for ( int i = 0; i < static_cast<int>( floor.tiles.size() ) && i < CELLS_PER_CHUNK; ++i ) {
            if ( floor.tiles[ i ] != 0 ) {
                world.setTile( originX + i % WorldConstants::CHUNK_SIZE, originY + i / WorldConstants::CHUNK_SIZE, floor.z, floor.tiles[ i ] );
            }
        }

        for ( const Cell& overlay : floor.overlays ) {
            world.setOverlay( originX + overlay.x, originY + overlay.y, floor.z, overlay.type );
        }

        for ( const Cell& object : floor.objects ) {
            world.setObject( originX + object.x, originY + object.y, floor.z, object.type );
        }
    }
}

int ChunkDTO::chunkX() const {
    return _chunkX;
}

void ChunkDTO::setChunkX( int chunkX ) {
    _chunkX = chunkX;
}

int ChunkDTO::chunkY() const {
    return _chunkY;
}

void ChunkDTO::setChunkY( int chunkY ) {
    _chunkY = chunkY;
}

const std::vector<ChunkDTO::Floor>& ChunkDTO::floors() const {
    return _floors;
}

void ChunkDTO::addFloor( const Floor& floor ) {
    _floors.push_back( floor );
}

} // namespace Engine
