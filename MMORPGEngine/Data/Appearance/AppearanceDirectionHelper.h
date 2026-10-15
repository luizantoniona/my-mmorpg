#ifndef APPEARANCEDIRECTIONHELPER_H
#define APPEARANCEDIRECTIONHELPER_H

#include <string>

#include <MMORPGEngine/Data/Appearance/AppearanceDirectionEnum.h>

namespace Engine {

class AppearanceDirectionHelper {
public:
    static std::string toString( AppearanceDirectionEnum direction );
    static AppearanceDirectionEnum fromString( const std::string& value );

    static AppearanceDirectionEnum fromDelta( int dx, int dy, AppearanceDirectionEnum fallback );
};

} // namespace Engine

#endif // APPEARANCEDIRECTIONHELPER_H
