#include "SkillFactory.h"

#include <unordered_set>

#include <QDebug>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/DataFactory.h>
#include <MMORPGEngine/Data/Skill/SkillNodeModel.h>

namespace Engine {

void SkillFactory::createSkillCatalog( const QString& configPath, const ItemTypeCatalog& itemTypeCatalog, SkillCatalog& skillCatalog ) {
    const QString mapPath = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( mapPath + "Map.json" );

    const QString itemTypesFile = mapPath + QString( mapJson[ "Catalogs" ][ "ItemTypes" ].asCString() );

    qInfo() << "SkillFactory::createSkillCatalog" << "[ITEM_TYPES_FILE_PATH]" << itemTypesFile;

    Json::Value json = JsonHelper::loadJsonFile( itemTypesFile );

    const Json::Value& itemTypes = json[ "ItemTypes" ];

    for ( const Json::Value& itemTypeJson : itemTypes ) {

        if ( !itemTypeJson.isMember( "SkillTree" ) ) {
            continue;
        }

        const uint32_t itemTypeId = itemTypeJson[ "Type" ].asUInt();
        if ( itemTypeCatalog.itemType( itemTypeId ) == nullptr ) {
            qWarning() << "SkillFactory::createSkillCatalog" << "Unknown ItemType, skipping SkillTree:" << itemTypeId;
            continue;
        }

        SkillTreeModel skillTree;
        skillTree.setItemType( itemTypeId );

        const Json::Value& nodesJson = itemTypeJson[ "SkillTree" ];
        bool isTreeValid = true;

        for ( const Json::Value& nodeJson : nodesJson ) {

            if ( !nodeJson.isMember( "Type" ) || !nodeJson.isMember( "Name" ) ) {
                qWarning() << "SkillFactory::createSkillCatalog" << "Invalid node, skipping SkillTree:" << itemTypeId;
                isTreeValid = false;
                break;
            }

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

        if ( !isTreeValid ) {
            continue;
        }

        skillCatalog.addTree( std::move( skillTree ) );
    }

    qInfo() << "SkillFactory::createSkillCatalog";
}

void SkillFactory::saveSkillCatalog( const QString& configPath, const SkillCatalog& skillCatalog ) {
    const QString path = DataFactory::mapPath( configPath );

    Json::Value mapJson = JsonHelper::loadJsonFile( path + "Map.json" );

    const QString itemTypesFile = path + QString( mapJson[ "Catalogs" ][ "ItemTypes" ].asCString() );

    Json::Value json = JsonHelper::loadJsonFile( itemTypesFile );

    std::unordered_set<uint32_t> matchedTypes;

    for ( Json::Value& itemTypeJson : json[ "ItemTypes" ] ) {
        if ( !itemTypeJson.isMember( "Type" ) ) {
            continue;
        }

        const uint32_t itemTypeId = itemTypeJson[ "Type" ].asUInt();
        const SkillTreeModel* skillTree = skillCatalog.tree( itemTypeId );

        if ( skillTree == nullptr ) {
            itemTypeJson.removeMember( "SkillTree" );
            continue;
        }

        matchedTypes.insert( itemTypeId );

        Json::Value nodesJson( Json::arrayValue );
        for ( const auto& nodeEntry : skillTree->nodes() ) {
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

        itemTypeJson[ "SkillTree" ] = nodesJson;
    }

    for ( const auto& treeEntry : skillCatalog.trees() ) {
        if ( matchedTypes.find( treeEntry.first ) == matchedTypes.end() ) {
            qWarning() << "SkillFactory::saveSkillCatalog" << "Unknown ItemType, skipping SkillTree:" << treeEntry.first;
        }
    }

    qInfo() << "SkillFactory::saveSkillCatalog" << "[ITEM_TYPES_FILE_PATH]" << itemTypesFile;

    JsonHelper::saveJsonFile( itemTypesFile, json );
}

} // namespace Engine
