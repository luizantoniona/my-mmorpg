#include "ManifestDataModel.h"

namespace Engine {

ManifestDataModel::ManifestDataModel() :
    _path( "" ),
    _hash( "" ) {
}

ManifestDataModel::ManifestDataModel( const std::string& path, const std::string& hash ) :
    _path( path ),
    _hash( hash ) {
}

std::string ManifestDataModel::path() const {
    return _path;
}

void ManifestDataModel::setPath( const std::string& path ) {
    _path = path;
}

std::string ManifestDataModel::hash() const {
    return _hash;
}

void ManifestDataModel::setHash( const std::string& hash ) {
    _hash = hash;
}

} // namespace Engine
