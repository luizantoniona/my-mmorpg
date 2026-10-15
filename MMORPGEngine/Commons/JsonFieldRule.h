#ifndef JSONFIELDRULE_H
#define JSONFIELDRULE_H

#include <MMORPGEngine/Commons/JsonFieldTypeEnum.h>

namespace Engine {

class JsonFieldRule {
public:
    JsonFieldRule( const char* key, JsonFieldTypeEnum type );

    const char* key;
    JsonFieldTypeEnum type;
};

} // namespace Engine

#endif // JSONFIELDRULE_H
