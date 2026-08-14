#include "RegisterTypes.h"

#include <QQmlEngine>

#include "Application/World/WorldPageControl.h"

namespace Editor {

void RegisterTypes::registerTypes() {
    qmlRegisterType<WorldPageControl>( "MMORPGEditorControls", 1, 0, "WorldPageControl" );
}

} // namespace Editor
