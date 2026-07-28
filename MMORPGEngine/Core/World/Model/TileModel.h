#ifndef TILEMODEL_H
#define TILEMODEL_H

#include <cstdint>

namespace Engine {

class TileModel {
public:
    TileModel();

    uint16_t groundId() const;
    void setGroundId( uint16_t groundId );

private:
    uint16_t _groundId;
};

} // namespace Engine

#endif // TILEMODEL_H
