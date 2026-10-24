#include "AttackCellModel.h"

namespace Engine {

AttackCellModel::AttackCellModel( int forward, int lateral ) :
    _forward( forward ),
    _lateral( lateral ) {
}

int AttackCellModel::forward() const {
    return _forward;
}

int AttackCellModel::lateral() const {
    return _lateral;
}

} // namespace Engine
