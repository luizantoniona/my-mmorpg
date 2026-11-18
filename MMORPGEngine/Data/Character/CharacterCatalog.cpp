#include "CharacterCatalog.h"

#include <algorithm>

#include <MMORPGEngine/Entity/Character/CharacterAppearanceFeatureHelper.h>

namespace Engine {

CharacterCatalog::CharacterCatalog() :
    _frameSize(),
    _rows(),
    _bodies() {
}

const QSize& CharacterCatalog::frameSize() const {
    return _frameSize;
}

void CharacterCatalog::setFrameSize( const QSize& frameSize ) {
    _frameSize = frameSize;
}

const std::map<AppearanceDirectionEnum, int>& CharacterCatalog::rows() const {
    return _rows;
}

void CharacterCatalog::setRow( AppearanceDirectionEnum direction, int row ) {
    _rows.insert_or_assign( direction, row );
}

int CharacterCatalog::rowCount() const {
    int maxRow = -1;

    for ( const auto& entry : _rows ) {
        maxRow = std::max( maxRow, entry.second );
    }

    return maxRow + 1;
}

const CharacterBodyModel* CharacterCatalog::body( const QString& id ) const {
    const auto iterator = _bodies.find( id );

    if ( iterator == _bodies.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

const std::map<QString, CharacterBodyModel>& CharacterCatalog::bodies() const {
    return _bodies;
}

void CharacterCatalog::addBody( const CharacterBodyModel& body ) {
    _bodies.insert( { body.id(), body } );
}

std::vector<CharacterAppearanceFeatureEnum> CharacterCatalog::features() const {
    std::vector<CharacterAppearanceFeatureEnum> available;

    for ( const CharacterAppearanceFeatureEnum feature : CharacterAppearanceFeatureHelper::all() ) {
        switch ( feature ) {
        case CharacterAppearanceFeatureEnum::BODY:
            if ( !_bodies.empty() ) {
                available.push_back( feature );
            }
            break;
        case CharacterAppearanceFeatureEnum::UNKNOWN:
            break;
        }
    }

    return available;
}

std::vector<CharacterOptionModel> CharacterCatalog::options( CharacterAppearanceFeatureEnum feature, const CharacterAppearanceModel& selection ) const {
    std::vector<CharacterOptionModel> options;

    switch ( feature ) {
    case CharacterAppearanceFeatureEnum::BODY:
        for ( const auto& entry : _bodies ) {
            options.push_back( CharacterOptionModel( entry.second.id(), entry.second.name() ) );
        }
        break;
    case CharacterAppearanceFeatureEnum::UNKNOWN:
        break;
    }

    return options;
}

bool CharacterCatalog::isRequired( CharacterAppearanceFeatureEnum feature ) const {
    switch ( feature ) {
    case CharacterAppearanceFeatureEnum::BODY:
        return true;
    case CharacterAppearanceFeatureEnum::UNKNOWN:
        return false;
    }

    return false;
}

bool CharacterCatalog::isValidAppearance( const CharacterAppearanceModel& appearance ) const {
    if ( appearance.has( CharacterAppearanceFeatureEnum::UNKNOWN ) ) {
        return false;
    }

    for ( const CharacterAppearanceFeatureEnum feature : CharacterAppearanceFeatureHelper::all() ) {
        if ( !appearance.has( feature ) ) {
            if ( isRequired( feature ) ) {
                return false;
            }

            continue;
        }

        const std::vector<CharacterOptionModel> allowed = options( feature, appearance );
        const QString id = appearance.id( feature );

        const bool isAllowed = std::any_of( allowed.begin(), allowed.end(), [ &id ]( const CharacterOptionModel& option ) {
            return option.id() == id;
        } );

        if ( !isAllowed ) {
            return false;
        }
    }

    return true;
}

bool CharacterCatalog::isEmpty() const {
    return _bodies.empty();
}

} // namespace Engine
