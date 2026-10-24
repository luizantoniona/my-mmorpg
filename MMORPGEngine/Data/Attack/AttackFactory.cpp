#include "AttackFactory.h"

#include <string>
#include <vector>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/Attack/AttackHitModeHelper.h>

namespace Engine {

namespace {

constexpr char ORIGIN_MARK = '@';
constexpr char CELL_MARK = 'X';

bool isEmptyMark( char mark ) {
    return mark == ' ' || mark == '.';
}

bool parseShape( const Json::Value& shapeJson, std::vector<AttackCellModel>& cells ) {
    if ( !shapeJson.isArray() || shapeJson.empty() ) {
        return false;
    }

    std::vector<std::string> rows;
    for ( const Json::Value& rowJson : shapeJson ) {
        if ( !rowJson.isString() ) {
            return false;
        }

        rows.push_back( rowJson.asString() );
    }

    int originRow = -1;
    int originColumn = -1;

    for ( int row = 0; row < static_cast<int>( rows.size() ); ++row ) {
        for ( int column = 0; column < static_cast<int>( rows[ row ].size() ); ++column ) {
            if ( rows[ row ][ column ] != ORIGIN_MARK ) {
                continue;
            }

            if ( originRow != -1 ) {
                return false;
            }

            originRow = row;
            originColumn = column;
        }
    }

    if ( originRow == -1 ) {
        return false;
    }

    for ( int row = 0; row < static_cast<int>( rows.size() ); ++row ) {
        for ( int column = 0; column < static_cast<int>( rows[ row ].size() ); ++column ) {
            const char mark = rows[ row ][ column ];

            if ( mark == ORIGIN_MARK || isEmptyMark( mark ) ) {
                continue;
            }

            if ( mark != CELL_MARK ) {
                return false;
            }

            cells.push_back( AttackCellModel( originRow - row, column - originColumn ) );
        }
    }

    return !cells.empty();
}

bool parseStep( const Json::Value& stepJson, AttackStepModel& step ) {
    if ( !stepJson.isObject() ) {
        return false;
    }

    if ( !JsonHelper::hasValidFields( stepJson, { { "Delay", JsonFieldTypeEnum::NUMBER }, { "Damage", JsonFieldTypeEnum::NUMBER }, { "Shape", JsonFieldTypeEnum::ARRAY } } ) ) {
        return false;
    }

    if ( !stepJson.isMember( "Delay" ) || !stepJson.isMember( "Damage" ) || !stepJson.isMember( "Shape" ) ) {
        return false;
    }

    std::vector<AttackCellModel> cells;
    if ( !parseShape( stepJson[ "Shape" ], cells ) ) {
        return false;
    }

    step.setCells( cells );
    step.setDelaySeconds( stepJson[ "Delay" ].asDouble() );
    step.setDamageMultiplier( stepJson[ "Damage" ].asDouble() );

    return step.delaySeconds() >= 0.0 && step.damageMultiplier() > 0.0;
}

} // namespace

bool AttackFactory::fromJson( const Json::Value& json, AttackModel& attack ) {
    if ( !json.isObject() ) {
        return false;
    }

    if ( !JsonHelper::hasValidFields( json, { { "CastSeconds", JsonFieldTypeEnum::NUMBER }, { "HitMode", JsonFieldTypeEnum::STRING }, { "Steps", JsonFieldTypeEnum::ARRAY } } ) ) {
        return false;
    }

    if ( !json.isMember( "CastSeconds" ) || !json.isMember( "HitMode" ) || !json.isMember( "Steps" ) || json[ "Steps" ].empty() ) {
        return false;
    }

    AttackModel parsed;
    parsed.setCastSeconds( json[ "CastSeconds" ].asDouble() );
    parsed.setHitMode( AttackHitModeHelper::fromString( json[ "HitMode" ].asString() ) );

    if ( parsed.castSeconds() <= 0.0 || parsed.hitMode() == AttackHitModeEnum::UNKNOWN ) {
        return false;
    }

    std::vector<AttackStepModel> steps;

    for ( const Json::Value& stepJson : json[ "Steps" ] ) {
        AttackStepModel step;

        if ( !parseStep( stepJson, step ) ) {
            return false;
        }

        steps.push_back( step );
    }

    parsed.setSteps( steps );
    attack = parsed;

    return true;
}

} // namespace Engine
