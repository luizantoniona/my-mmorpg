#include "RegisterEditorTypes.h"

#include <QQmlEngine>

#include <MMORPGEditor/Editor/Application/World/EditorRenderWorld.h>
#include <MMORPGEditor/Editor/Application/World/WorldPageControl.h>

namespace Editor {

void RegisterEditorTypes::registerTypes() {
    qmlRegisterType<EditorRenderWorld>( "MMORPGEditorComponents", 1, 0, "EditorRenderWorld" );
    qmlRegisterType<WorldPageControl>( "MMORPGEditorControls", 1, 0, "WorldPageControl" );
}

} // namespace Editor
