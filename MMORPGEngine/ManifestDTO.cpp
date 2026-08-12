#include "ManifestDTO.h"

namespace Engine {

ManifestDTO::ManifestDTO() = default;

ManifestDTO::ManifestDTO( const ManifestModel& manifest ) {
    for ( const auto& data : manifest.data() ) {
        _data.emplace_back( data );
    }
}

ManifestDTO::~ManifestDTO() = default;

ManifestDTO ManifestDTO::fromJson( const Json::Value& json ) {
    ManifestDTO manifestDTO;

    if ( !json.isObject() ) {
        return manifestDTO;
    }

    if ( !json.isMember( "data" ) || !json[ "data" ].isArray() ) {
        return manifestDTO;
    }

    for ( const auto& dataJson : json[ "data" ] ) {
        manifestDTO._data.emplace_back( ManifestDataDTO::fromJson( dataJson ) );
    }

    return manifestDTO;
}

Json::Value ManifestDTO::toJson() const {
    Json::Value json;
    Json::Value dataJson( Json::arrayValue );

    for ( const auto& data : _data ) {
        dataJson.append( data.toJson() );
    }

    json[ "data" ] = dataJson;

    return json;
}

bool ManifestDTO::isValid() {
    // TODO: Create a minimal DTO validation
    return true;
}

} // namespace Engine
