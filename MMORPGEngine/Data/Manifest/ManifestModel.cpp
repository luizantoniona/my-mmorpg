#include "ManifestModel.h"

namespace Engine {

ManifestModel::ManifestModel() :
    _data() {
}

const std::vector<ManifestDataModel>& ManifestModel::data() const {
    return _data;
}

void ManifestModel::addData( const ManifestDataModel& data ) {
    _data.push_back( data );
}

void ManifestModel::clear() {
    _data.clear();
}

} // namespace Engine
