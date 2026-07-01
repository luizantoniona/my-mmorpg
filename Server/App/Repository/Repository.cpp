#include "Repository.h"

#include <Engine/Commons/Singleton.h>

#include "Database.h"

namespace Engine {

Repository::Repository() :
    _db( Singleton<Database>::instance().database() ) {
}

} // namespace Engine
