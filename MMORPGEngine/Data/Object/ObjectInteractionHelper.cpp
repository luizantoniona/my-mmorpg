#include "ObjectInteractionHelper.h"

namespace Engine {

std::string ObjectInteractionHelper::toString( ObjectInteractionEnum type ) {
    switch ( type ) {
    case ObjectInteractionEnum::CONTAINER:
        return "CONTAINER";
    }

    return "";
}

std::optional<ObjectInteractionEnum> ObjectInteractionHelper::fromString( const std::string& value ) {
    if ( value == "CONTAINER" ) {
        return ObjectInteractionEnum::CONTAINER;
    }

    return std::nullopt;
}

} // namespace Engine
