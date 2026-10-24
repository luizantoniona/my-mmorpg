#ifndef ATTACKHITMODEHELPER_H
#define ATTACKHITMODEHELPER_H

#include <string>

#include <MMORPGEngine/Data/Attack/AttackHitModeEnum.h>

namespace Engine {

class AttackHitModeHelper {
public:
    static std::string toString( AttackHitModeEnum hitMode );
    static AttackHitModeEnum fromString( const std::string& value );
};

} // namespace Engine

#endif // ATTACKHITMODEHELPER_H
