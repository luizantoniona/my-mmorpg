#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include <cstdint>
#include <unordered_map>

#include <MMORPGEngine/World/FloorModel.h>
#include <MMORPGEngine/World/GroundCatalog.h>

namespace Engine {

class WorldModel {
public:
    WorldModel();

    std::string name() const;
    void setName( const std::string& name );

    uint16_t width() const;
    void setWidth( uint16_t width );

    uint16_t height() const;
    void setHeight( uint16_t height );

    const GroundCatalog& groundCatalog() const;
    GroundCatalog& groundCatalog();

    const std::unordered_map<int32_t, FloorModel>& floors() const;
    std::unordered_map<int32_t, FloorModel>& floors();

    FloorModel* floor( int32_t z );
    const FloorModel* floor( int32_t z ) const;

private:
    std::string _name;
    uint16_t _width;
    uint16_t _height;

    // --- Catalogs ---
    GroundCatalog _groundCatalog;

    // --- World structure ---
    std::unordered_map<int32_t, FloorModel> _floors;
};

} // namespace Engine

#endif // WORLDMODEL_H
