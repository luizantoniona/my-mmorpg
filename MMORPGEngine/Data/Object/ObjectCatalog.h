#ifndef OBJECTCATALOG_H
#define OBJECTCATALOG_H

#include <unordered_map>

#include <MMORPGEngine/Data/Object/ObjectModel.h>

namespace Engine {

class ObjectCatalog {
public:
    ObjectCatalog();

    const ObjectModel* object( uint32_t type ) const;
    const std::unordered_map<uint32_t, ObjectModel>& objects() const;
    void addObject( const ObjectModel& object );

private:
    std::unordered_map<uint32_t, ObjectModel> _objects;
};

} // namespace Engine

#endif // OBJECTCATALOG_H
