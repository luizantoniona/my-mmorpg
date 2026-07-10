#include "RegisterTypes.h"

#include <QQmlEngine>

#include "Application/EditorPageControl.h"

void RegisterTypes::registerTypes() {
    qmlRegisterType<EditorPageControl>( "MMORPGEditorControls", 1, 0, "EditorPageControl" );
}
