#include "WorldModel.h"

namespace Engine {

WorldModel::WorldModel() :
    _name( "" ),
    _groundCatalog() {}

std::string WorldModel::name() const {
    return _name;
}

void WorldModel::setName( const std::string& name ) {
    _name = name;
}

const GroundCatalog& WorldModel::groundCatalog() const {
    return _groundCatalog;
}

GroundCatalog& WorldModel::groundCatalog() {
    return _groundCatalog;
}

} // namespace Engine
