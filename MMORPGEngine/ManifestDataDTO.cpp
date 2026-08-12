#include "ManifestDataDTO.h"

namespace Engine {

ManifestDataDTO::ManifestDataDTO() :
    _path( "" ),
    _hash( "" ) {
}

ManifestDataDTO::ManifestDataDTO( const ManifestDataModel& data ) :
    _path( data.path() ),
    _hash( data.hash() ) {
}

ManifestDataDTO::~ManifestDataDTO() = default;

ManifestDataDTO ManifestDataDTO::fromJson( const Json::Value& json ) {

    ManifestDataDTO dataDTO;

    if ( !json.isObject() ) {
        return dataDTO;
    }

    if ( json.isMember( "path" ) && json[ "path" ].isString() ) {
        dataDTO.setPath( json[ "path" ].asString() );
    }

    if ( json.isMember( "hash" ) && json[ "hash" ].isString() ) {
        dataDTO.setHash( json[ "hash" ].asString() );
    }

    return dataDTO;
}

Json::Value ManifestDataDTO::toJson() const {
    Json::Value json;
    json[ "path" ] = _path;
    json[ "hash" ] = _hash;
    return json;
}

const std::string& ManifestDataDTO::path() const {
    return _path;
}

void ManifestDataDTO::setPath( const std::string& path ) {
    _path = path;
}

const std::string& ManifestDataDTO::hash() const {
    return _hash;
}

void ManifestDataDTO::setHash( const std::string& hash ) {
    _hash = hash;
}

} // namespace Engine
