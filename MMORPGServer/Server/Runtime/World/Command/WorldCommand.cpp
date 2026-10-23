#include "WorldCommand.h"

namespace Server {

WorldCommand::WorldCommand( WorldCommandTypeEnum type ) :
    _type( type ) {
}

WorldCommand::~WorldCommand() = default;

WorldCommandTypeEnum WorldCommand::type() const {
    return _type;
}

} // namespace Server
