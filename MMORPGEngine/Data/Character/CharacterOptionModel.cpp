#include "CharacterOptionModel.h"

namespace Engine {

CharacterOptionModel::CharacterOptionModel() :
    _id(),
    _name() {
}

CharacterOptionModel::CharacterOptionModel( const QString& id, const QString& name ) :
    _id( id ),
    _name( name ) {
}

const QString& CharacterOptionModel::id() const {
    return _id;
}

const QString& CharacterOptionModel::name() const {
    return _name;
}

} // namespace Engine
