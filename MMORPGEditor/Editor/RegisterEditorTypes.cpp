#include "RegisterEditorTypes.h"

#include <QQmlEngine>

#include <MMORPGEditor/Editor/Application/World/Object/ObjectSelectionControl.h>
#include <MMORPGEditor/Editor/Application/World/Tile/TileSelectionControl.h>
#include <MMORPGEditor/Editor/Application/World/WorldControl.h>
#include <MMORPGEditor/Editor/Renderer/EditorRenderWorld.h>

namespace Editor {

void RegisterEditorTypes::registerTypes() {
    // --- Controls
    qmlRegisterType<ObjectSelectionControl>( "MMORPGEditorControls", 1, 0, "ObjectSelectionControl" );
    qmlRegisterType<TileSelectionControl>( "MMORPGEditorControls", 1, 0, "TileSelectionControl" );
    qmlRegisterType<WorldControl>( "MMORPGEditorControls", 1, 0, "WorldPageControl" );

    // --- Others Components
    qmlRegisterType<EditorRenderWorld>( "MMORPGEditorComponents", 1, 0, "EditorRenderWorld" );
}

} // namespace Editor
