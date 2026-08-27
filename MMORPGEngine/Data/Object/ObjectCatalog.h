#ifndef OBJECTCATALOG_H
#define OBJECTCATALOG_H

#include <unordered_map>

#include <MMORPGEngine/Data/Object/ObjectModel.h>

namespace Engine {

class ObjectCatalog {
public:
    ObjectCatalog();

    const ObjectModel* texture( uint32_t type ) const;
    const std::unordered_map<uint32_t, ObjectModel>& textures() const;
    void addTexture( const ObjectModel& object );

private:
    std::unordered_map<uint32_t, ObjectModel> _objects;
};

} // namespace Engine

#endif // OBJECTCATALOG_H
