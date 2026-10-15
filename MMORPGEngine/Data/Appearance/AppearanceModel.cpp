#include "AppearanceModel.h"

namespace Engine {

AppearanceModel::AppearanceModel() :
    _animations() {
}

bool AppearanceModel::isNull() const {
    return _animations.empty();
}

void AppearanceModel::setAnimation( AppearanceStateEnum state, AppearanceDirectionEnum direction, const AnimationModel& animation ) {
    _animations.insert_or_assign( std::make_pair( state, direction ), animation );
}

const AnimationModel& AppearanceModel::animation( AppearanceStateEnum state, AppearanceDirectionEnum direction ) const {
    static const AnimationModel EMPTY;

    if ( const AnimationModel* animation = find( state, direction ) ) {
        return *animation;
    }

    if ( const AnimationModel* animation = find( AppearanceStateEnum::IDLE, direction ) ) {
        return *animation;
    }

    return EMPTY;
}

const AnimationModel* AppearanceModel::find( AppearanceStateEnum state, AppearanceDirectionEnum direction ) const {
    const auto it = _animations.find( std::make_pair( state, direction ) );

    return it == _animations.end() ? nullptr : &it->second;
}

} // namespace Engine
