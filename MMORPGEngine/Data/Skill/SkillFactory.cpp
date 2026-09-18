#include "SkillFactory.h"

#include <QDebug>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/DataFactory.h>
#include <MMORPGEngine/Data/Skill/SkillNodeModel.h>

namespace Engine {

void SkillFactory::createSkillCatalog( const QString& configPath, const ItemTypeCatalog& itemTypeCatalog, SkillCatalog& skillCatalog ) {
    const QString mapPath = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    const QString skillTreesFile = mapPath + QString( mapJson[ "Catalogs" ][ "SkillTrees" ].asCString() );

    qInfo() << "SkillFactory::createSkillCatalog" << "[SKILL_TREES_FILE_PATH]" << skillTreesFile;

    Json::Value json = JsonHelper::loadJsonFile( skillTreesFile );

    const Json::Value& skillTrees = json[ "SkillTrees" ];

    for ( const Json::Value& skillTreeJson : skillTrees ) {

        const uint32_t itemTypeId = skillTreeJson[ "ItemType" ].asUInt();
        if ( itemTypeCatalog.itemType( itemTypeId ) == nullptr ) {
            qWarning() << "SkillFactory::createSkillCatalog" << "Unknown ItemType, skipping SkillTree:" << itemTypeId;
            continue;
        }

        SkillTreeModel skillTree;
        skillTree.setItemType( itemTypeId );

        const Json::Value& nodesJson = skillTreeJson[ "Nodes" ];
        for ( const Json::Value& nodeJson : nodesJson ) {

            SkillNodeModel node;
            node.setType( nodeJson[ "Type" ].asUInt() );
            node.setName( QString( nodeJson[ "Name" ].asCString() ) );
            node.setProficiencyLevel( nodeJson.get( "ProficiencyLevel", 0 ).asUInt() );

            QList<uint32_t> prerequisites;
            const Json::Value& prerequisitesJson = nodeJson[ "Prerequisites" ];
            if ( prerequisitesJson.isArray() ) {
                for ( const Json::Value& prerequisiteJson : prerequisitesJson ) {
                    prerequisites.append( prerequisiteJson.asUInt() );
                }
            }
            node.setPrerequisites( prerequisites );

            skillTree.addNode( std::move( node ) );
        }

        skillCatalog.addTree( std::move( skillTree ) );
    }

    qInfo() << "SkillFactory::createSkillCatalog";
}

void SkillFactory::saveSkillCatalog( const QString& configPath, const SkillCatalog& skillCatalog ) {
    const QString path = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( path + "Map.json" );

    const QString skillTreesFile = path + QString( mapJson[ "Catalogs" ][ "SkillTrees" ].asCString() );

    Json::Value json;
    json[ "SkillTrees" ] = Json::Value( Json::arrayValue );

    for ( const auto& treeEntry : skillCatalog.trees() ) {
        const SkillTreeModel& skillTree = treeEntry.second;

        Json::Value skillTreeJson;
        skillTreeJson[ "ItemType" ] = skillTree.itemType();

        Json::Value nodesJson( Json::arrayValue );
        for ( const auto& nodeEntry : skillTree.nodes() ) {
            const SkillNodeModel& node = nodeEntry.second;

            Json::Value nodeJson;
            nodeJson[ "Type" ] = node.type();
            nodeJson[ "Name" ] = node.name().toStdString();
            nodeJson[ "ProficiencyLevel" ] = node.proficiencyLevel();

            Json::Value prerequisitesJson( Json::arrayValue );
            for ( uint32_t prerequisite : node.prerequisites() ) {
                prerequisitesJson.append( prerequisite );
            }
            nodeJson[ "Prerequisites" ] = prerequisitesJson;

            nodesJson.append( nodeJson );
        }
        skillTreeJson[ "Nodes" ] = nodesJson;

        json[ "SkillTrees" ].append( skillTreeJson );
    }

    qInfo() << "SkillFactory::saveSkillCatalog" << "[SKILL_TREES_FILE_PATH]" << skillTreesFile;

    JsonHelper::saveJsonFile( skillTreesFile, json );
}

} // namespace Engine
