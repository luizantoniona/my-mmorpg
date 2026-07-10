#include "Repository.h"

#include <Database/Database.h>
#include <MMORPGEngine/Commons/Singleton.h>

namespace Server {

Repository::Repository() :
    _db( Engine::Singleton<Database>::instance().database() ) {}

} // namespace Server
