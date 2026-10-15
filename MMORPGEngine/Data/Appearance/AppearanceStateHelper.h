#ifndef APPEARANCESTATEHELPER_H
#define APPEARANCESTATEHELPER_H

#include <string>

#include <MMORPGEngine/Data/Appearance/AppearanceStateEnum.h>

namespace Engine {

class AppearanceStateHelper {
public:
    static std::string toString( AppearanceStateEnum state );
    static AppearanceStateEnum fromString( const std::string& value );
};

} // namespace Engine

#endif // APPEARANCESTATEHELPER_H
