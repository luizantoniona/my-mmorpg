#include "CharacterBodyModel.h"

namespace Engine {

CharacterBodyModel::CharacterBodyModel() :
    _id(),
    _name(),
    _sprite(),
    _clips() {
}

const QString& CharacterBodyModel::id() const {
    return _id;
}

void CharacterBodyModel::setId( const QString& id ) {
    _id = id;
}

const QString& CharacterBodyModel::name() const {
    return _name;
}

void CharacterBodyModel::setName( const QString& name ) {
    _name = name;
}

const CharacterSpriteModel& CharacterBodyModel::sprite() const {
    return _sprite;
}

CharacterSpriteModel& CharacterBodyModel::sprite() {
    return _sprite;
}

const std::map<AppearanceStateEnum, CharacterClipModel>& CharacterBodyModel::clips() const {
    return _clips;
}

const CharacterClipModel* CharacterBodyModel::clip( AppearanceStateEnum state ) const {
    const auto iterator = _clips.find( state );

    if ( iterator == _clips.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

void CharacterBodyModel::setClip( AppearanceStateEnum state, const CharacterClipModel& clip ) {
    _clips.insert_or_assign( state, clip );
}

} // namespace Engine
