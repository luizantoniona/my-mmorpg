#include "RegisterEditorTypes.h"

#include <QQmlEngine>

#include "Application/World/WorldPageControl.h"

namespace Editor {

void RegisterEditorTypes::registerTypes() {
    qmlRegisterType<WorldPageControl>( "MMORPGEditorControls", 1, 0, "WorldPageControl" );
}

} // namespace Editor
