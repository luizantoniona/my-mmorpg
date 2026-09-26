#ifndef CREATURESPAWNAREAMODEL_H
#define CREATURESPAWNAREAMODEL_H

#include <cstdint>
#include <vector>

#include <MMORPGEngine/Data/Creature/CreatureSpawnEntryModel.h>

namespace Engine {

class CreatureSpawnAreaModel {
public:
    CreatureSpawnAreaModel();

    int x() const;
    void setX( int x );

    int y() const;
    void setY( int y );

    uint32_t width() const;
    void setWidth( uint32_t width );

    uint32_t height() const;
    void setHeight( uint32_t height );

    const std::vector<CreatureSpawnEntryModel>& creatures() const;
    void setCreatures( const std::vector<CreatureSpawnEntryModel>& creatures );

private:
    std::vector<CreatureSpawnEntryModel> _creatures;
    int _x;
    int _y;
    uint32_t _width;
    uint32_t _height;
};

} // namespace Engine

#endif // CREATURESPAWNAREAMODEL_H
