#ifndef MANIFESTMODEL_H
#define MANIFESTMODEL_H

#include <vector>

#include <MMORPGEngine/ManifestDataModel.h>

namespace Engine {

class ManifestModel {
public:
    ManifestModel();

    const std::vector<ManifestDataModel>& data() const;
    void addData( const ManifestDataModel& data );
    void clear();

private:
    std::vector<ManifestDataModel> _data;
};

} // namespace Engine

#endif // FILEMANIFESTMODEL_H
