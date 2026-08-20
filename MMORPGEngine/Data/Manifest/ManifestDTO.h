#ifndef MANIFESTDTO_H
#define MANIFESTDTO_H

#include <vector>

#include <MMORPGEngine/Data/Manifest/ManifestDataDTO.h>
#include <MMORPGEngine/Data/Manifest/ManifestModel.h>

namespace Engine {

class ManifestDTO {
public:
    ManifestDTO();
    explicit ManifestDTO( const ManifestModel& manifest );
    ~ManifestDTO();

    static ManifestDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    bool isValid();

private:
    std::vector<ManifestDataDTO> _data;
};

} // namespace Engine

#endif // MANIFESTDTO_H
