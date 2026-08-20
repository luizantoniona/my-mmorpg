#ifndef TILEMODEL_H
#define TILEMODEL_H

#include <cstdint>

namespace Engine {

class TileModel {
public:
    TileModel();

    uint32_t tileTextureId() const;
    void setTileTextureId( uint32_t tileTextureId );

private:
    uint32_t _tileTextureId;
};

} // namespace Engine

#endif // TILEMODEL_H
