#ifndef OBJECTINTERACTIONHELPER_H
#define OBJECTINTERACTIONHELPER_H

#include <optional>
#include <string>

#include <MMORPGEngine/Data/Object/ObjectInteractionEnum.h>

namespace Engine {

class ObjectInteractionHelper {
public:
    static std::string toString( ObjectInteractionEnum type );
    static std::optional<ObjectInteractionEnum> fromString( const std::string& value );
};

} // namespace Engine

#endif // OBJECTINTERACTIONHELPER_H
