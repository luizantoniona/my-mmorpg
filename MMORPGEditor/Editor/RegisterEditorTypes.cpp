#include "RegisterEditorTypes.h"

#include <QQmlEngine>

#include <MMORPGEditor/Editor/Application/Configuration/ConfigurationControl.h>
#include <MMORPGEditor/Editor/Application/Objects/ObjectCreationControl.h>
#include <MMORPGEditor/Editor/Application/Tiles/TileCreationControl.h>
#include <MMORPGEditor/Editor/Application/World/Object/ObjectPaletteModel.h>
#include <MMORPGEditor/Editor/Application/World/Object/ObjectSelectionControl.h>
#include <MMORPGEditor/Editor/Application/World/Tile/TilePaletteModel.h>
#include <MMORPGEditor/Editor/Application/World/Tile/TileSelectionControl.h>
#include <MMORPGEditor/Editor/Application/World/WorldControl.h>
#include <MMORPGEditor/Editor/Renderer/EditorRenderWorld.h>

namespace Editor {

void RegisterEditorTypes::registerTypes() {
    // --- Controls
    qmlRegisterType<ConfigurationControl>( "MMORPGEditorControls", 1, 0, "ConfigurationControl" );
    qmlRegisterType<ObjectCreationControl>( "MMORPGEditorControls", 1, 0, "ObjectCreationControl" );
    qmlRegisterType<ObjectSelectionControl>( "MMORPGEditorControls", 1, 0, "ObjectSelectionControl" );
    qmlRegisterType<TileCreationControl>( "MMORPGEditorControls", 1, 0, "TileCreationControl" );
    qmlRegisterType<TileSelectionControl>( "MMORPGEditorControls", 1, 0, "TileSelectionControl" );
    qmlRegisterType<WorldControl>( "MMORPGEditorControls", 1, 0, "WorldPageControl" );

    // --- Models
    qmlRegisterType<ObjectPaletteModel>( "MMORPGEditorControls", 1, 0, "ObjectPaletteModel" );
    qmlRegisterType<TilePaletteModel>( "MMORPGEditorControls", 1, 0, "TilePaletteModel" );

    // --- Others Components
    qmlRegisterType<EditorRenderWorld>( "MMORPGEditorComponents", 1, 0, "EditorRenderWorld" );
}

} // namespace Editor
