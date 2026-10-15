#include "GroundItemDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

GroundItemDTO::GroundItemDTO() :
    _idItem( "" ),
    _idGroundItem( 0 ),
    _x( 0 ),
    _y( 0 ),
    _z( 0 ) {
}

GroundItemDTO::~GroundItemDTO() = default;

GroundItemDTO GroundItemDTO::fromModel( const GroundItemModel* groundItem ) {
    GroundItemDTO dto;

    dto._idGroundItem = groundItem->idGroundItem();
    dto._idItem = groundItem->idItem();
    dto._x = groundItem->position().x();
    dto._y = groundItem->position().y();
    dto._z = groundItem->position().z();

    return dto;
}

GroundItemDTO GroundItemDTO::fromJson( const Json::Value& json ) {
    GroundItemDTO dto;

    if ( json.isMember( "idGroundItem" ) && json[ "idGroundItem" ].isInt() ) {
        dto._idGroundItem = json[ "idGroundItem" ].asInt();
    }

    if ( json.isMember( "idItem" ) && json[ "idItem" ].isString() ) {
        dto._idItem = QString::fromStdString( json[ "idItem" ].asString() );
    }

    if ( json.isMember( "x" ) && json[ "x" ].isInt() ) {
        dto._x = json[ "x" ].asInt();
    }

    if ( json.isMember( "y" ) && json[ "y" ].isInt() ) {
        dto._y = json[ "y" ].asInt();
    }

    if ( json.isMember( "z" ) && json[ "z" ].isInt() ) {
        dto._z = json[ "z" ].asInt();
    }

    return dto;
}

Json::Value GroundItemDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::GROUND_ITEM );
    json[ "idGroundItem" ] = _idGroundItem;
    json[ "idItem" ] = _idItem.toStdString();
    json[ "x" ] = _x;
    json[ "y" ] = _y;
    json[ "z" ] = _z;

    return json;
}

int GroundItemDTO::idGroundItem() const {
    return _idGroundItem;
}

void GroundItemDTO::setIdGroundItem( int idGroundItem ) {
    _idGroundItem = idGroundItem;
}

QString GroundItemDTO::idItem() const {
    return _idItem;
}

void GroundItemDTO::setIdItem( const QString& idItem ) {
    _idItem = idItem;
}

int GroundItemDTO::x() const {
    return _x;
}

void GroundItemDTO::setX( int x ) {
    _x = x;
}

int GroundItemDTO::y() const {
    return _y;
}

void GroundItemDTO::setY( int y ) {
    _y = y;
}

int GroundItemDTO::z() const {
    return _z;
}

void GroundItemDTO::setZ( int z ) {
    _z = z;
}

} // namespace Engine
