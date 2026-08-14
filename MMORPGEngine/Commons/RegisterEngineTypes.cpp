#include "RegisterEngineTypes.h"

#include <QQmlEngine>

#include <MMORPGEngine/Renderer/Viewport.h>

namespace Engine {

void RegisterEngineTypes::registerTypes() {
    qmlRegisterType<Engine::Viewport>( "MMORPGEngine", 1, 0, "Viewport" );
}

} // namespace Engine
