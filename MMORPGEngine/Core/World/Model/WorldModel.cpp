#include "WorldModel.h"

namespace Engine {

WorldModel::WorldModel() :
    _groundCatalog() {}

const GroundCatalog& WorldModel::groundCatalog() const {
    return _groundCatalog;
}

GroundCatalog& WorldModel::groundCatalog() {
    return _groundCatalog;
}

} // namespace Engine
