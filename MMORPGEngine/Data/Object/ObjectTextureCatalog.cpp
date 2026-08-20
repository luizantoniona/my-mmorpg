#include "ObjectTextureCatalog.h"

namespace Engine {

ObjectTextureCatalog::ObjectTextureCatalog() :
    _textures() {
}

const ObjectTextureModel* ObjectTextureCatalog::texture( uint32_t type ) const {
    auto iterator = _textures.find( type );

    if ( iterator == _textures.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

const std::unordered_map<uint32_t, ObjectTextureModel>& ObjectTextureCatalog::textures() const {
    return _textures;
}

void ObjectTextureCatalog::addTexture( const ObjectTextureModel& texture ) {
    _textures.insert( { texture.type(), texture } );
}

} // namespace Engine
