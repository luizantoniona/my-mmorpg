#ifndef MANIFESTDATADTO_H
#define MANIFESTDATADTO_H

#include <json/json.h>

#include <MMORPGEngine/ManifestDataModel.h>

namespace Engine {

class ManifestDataDTO {
public:
    ManifestDataDTO();
    explicit ManifestDataDTO( const ManifestDataModel& data );

    ~ManifestDataDTO();

    static ManifestDataDTO fromJson( const Json::Value& json );

    Json::Value toJson() const;

    const std::string& path() const;
    void setPath( const std::string& path );

    const std::string& hash() const;
    void setHash( const std::string& hash );

private:
    std::string _path;
    std::string _hash;
};

} // namespace Engine

#endif // MANIFESTDATADTO_H
