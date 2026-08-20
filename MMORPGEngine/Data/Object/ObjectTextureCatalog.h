#ifndef OBJECTTEXTURECATALOG_H
#define OBJECTTEXTURECATALOG_H

#include <unordered_map>

#include <MMORPGEngine/Data/Object/ObjectTextureModel.h>

namespace Engine {

class ObjectTextureCatalog {
public:
    ObjectTextureCatalog();

    const ObjectTextureModel* texture( uint32_t type ) const;
    const std::unordered_map<uint32_t, ObjectTextureModel>& textures() const;
    void addTexture( const ObjectTextureModel& texture );

private:
    std::unordered_map<uint32_t, ObjectTextureModel> _textures;
};

} // namespace Engine

#endif // OBJECTTEXTURECATALOG_H
