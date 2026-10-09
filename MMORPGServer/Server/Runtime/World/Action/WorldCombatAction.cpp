#include "WorldCombatAction.h"

namespace Server {

WorldCombatAction::WorldCombatAction( WorldRuntime& runtime ) :
    _runtime( runtime ) {
}

WorldCombatAction::~WorldCombatAction() = default;

} // namespace Server
