#ifndef MANIFESTDATAMODEL_H
#define MANIFESTDATAMODEL_H

#include <string>

namespace Engine {

class ManifestDataModel {
public:
    ManifestDataModel();
    ManifestDataModel( const std::string& path, const std::string& hash );

    std::string path() const;
    void setPath( const std::string& path );

    std::string hash() const;
    void setHash( const std::string& hash );

private:
    std::string _path;
    std::string _hash;
};

} // namespace Engine

#endif // MANIFESTDATAMODEL_H
