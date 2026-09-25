#ifndef MONSTERSPAWNAREAMODEL_H
#define MONSTERSPAWNAREAMODEL_H

#include <cstdint>
#include <vector>

#include <MMORPGEngine/Data/Monster/MonsterSpawnEntryModel.h>

namespace Engine {

class MonsterSpawnAreaModel {
public:
    MonsterSpawnAreaModel();

    int x() const;
    void setX( int x );

    int y() const;
    void setY( int y );

    uint32_t width() const;
    void setWidth( uint32_t width );

    uint32_t height() const;
    void setHeight( uint32_t height );

    const std::vector<MonsterSpawnEntryModel>& monsters() const;
    void setMonsters( const std::vector<MonsterSpawnEntryModel>& monsters );

private:
    std::vector<MonsterSpawnEntryModel> _monsters;
    int _x;
    int _y;
    uint32_t _width;
    uint32_t _height;
};

} // namespace Engine

#endif // MONSTERSPAWNAREAMODEL_H
