#include "Repository.h"

#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGServer/Server/Database/Database.h>

namespace Server {

Repository::Repository() :
    _db( Engine::Singleton<Database>::instance().database() ) {}

} // namespace Server
