#include "RegisterTypes.h"

#include <QQmlEngine>

#include "Application/World/WorldPageControl.h"

void RegisterTypes::registerTypes() {
    qmlRegisterType<WorldPageControl>( "MMORPGEditorControls", 1, 0, "WorldPageControl" );
}
