#include "HandRequirementHelper.h"

namespace Engine {

std::string HandRequirementHelper::toString( HandRequirementEnum handRequirement ) {
    switch ( handRequirement ) {
    case HandRequirementEnum::ONE_HANDED:
        return "ONE_HANDED";
    case HandRequirementEnum::TWO_HANDED:
        return "TWO_HANDED";
    }

    return "";
}

HandRequirementEnum HandRequirementHelper::fromString( const std::string& value ) {
    if ( value == "ONE_HANDED" ) {
        return HandRequirementEnum::ONE_HANDED;
    }
    if ( value == "TWO_HANDED" ) {
        return HandRequirementEnum::TWO_HANDED;
    }

    return HandRequirementEnum::UNKNOWN;
}

} // namespace Engine
