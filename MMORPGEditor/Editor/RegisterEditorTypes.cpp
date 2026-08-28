#include "RegisterEditorTypes.h"

#include <QQmlEngine>

#include <MMORPGEditor/Editor/Application/World/SelectionControl.h>
#include <MMORPGEditor/Editor/Application/World/WorldControl.h>
#include <MMORPGEditor/Editor/Renderer/EditorRenderWorld.h>

namespace Editor {

void RegisterEditorTypes::registerTypes() {
    // --- Controls
    qmlRegisterType<SelectionControl>( "MMORPGEditorControls", 1, 0, "SelectionControl" );
    qmlRegisterType<WorldControl>( "MMORPGEditorControls", 1, 0, "WorldPageControl" );

    // --- Others Components
    qmlRegisterType<EditorRenderWorld>( "MMORPGEditorComponents", 1, 0, "EditorRenderWorld" );
}

} // namespace Editor
