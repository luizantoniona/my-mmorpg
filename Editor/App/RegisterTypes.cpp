#include "RegisterTypes.h"

#include <QQmlEngine>

#include "Editor/EditorPageControl.h"

void RegisterTypes::registerTypes() {
    qmlRegisterType<EditorPageControl>( "MMORPGEditor.Controls", 1, 0, "EditorPageControl" );
}
