#ifndef HANDREQUIREMENTHELPER_H
#define HANDREQUIREMENTHELPER_H

#include <string>

#include <MMORPGEngine/Data/Item/HandRequirementEnum.h>

namespace Engine {

class HandRequirementHelper {
public:
    static std::string toString( HandRequirementEnum handRequirement );
    static HandRequirementEnum fromString( const std::string& value );
};

} // namespace Engine

#endif // HANDREQUIREMENTHELPER_H
