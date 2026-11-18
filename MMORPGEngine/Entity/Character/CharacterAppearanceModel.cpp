#include "CharacterAppearanceModel.h"

#include <MMORPGEngine/Entity/Character/CharacterAppearanceFeatureHelper.h>

namespace Engine {

CharacterAppearanceModel::CharacterAppearanceModel() :
    _selections() {
}

const std::map<CharacterAppearanceFeatureEnum, QString>& CharacterAppearanceModel::selections() const {
    return _selections;
}

bool CharacterAppearanceModel::has( CharacterAppearanceFeatureEnum feature ) const {
    return _selections.find( feature ) != _selections.end();
}

QString CharacterAppearanceModel::id( CharacterAppearanceFeatureEnum feature ) const {
    const auto iterator = _selections.find( feature );

    if ( iterator == _selections.end() ) {
        return QString();
    }

    return iterator->second;
}

void CharacterAppearanceModel::setId( CharacterAppearanceFeatureEnum feature, const QString& id ) {
    _selections.insert_or_assign( feature, id );
}

bool CharacterAppearanceModel::isEmpty() const {
    return _selections.empty();
}

QString CharacterAppearanceModel::key() const {
    QString key;

    for ( const auto& selection : _selections ) {
        key += QString::fromStdString( CharacterAppearanceFeatureHelper::toString( selection.first ) ) + "=" + selection.second + ";";
    }

    return key;
}

} // namespace Engine
