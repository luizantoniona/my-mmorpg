#include "SkillFactory.h"

#include <QDebug>
#include <QFile>

#include <json/json.h>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Data/DataFactory.h>
#include <MMORPGEngine/Data/Item/ItemTypeFactory.h>
#include <MMORPGEngine/Data/Skill/SkillNodeModel.h>

namespace Engine {

void SkillFactory::createSkillCatalog( const QString& configPath, const ItemTypeCatalog& itemTypeCatalog, SkillCatalog& skillCatalog ) {
    for ( const QString& itemTypeId : ItemTypeFactory::readTypeKeys( configPath ) ) {

        if ( itemTypeCatalog.itemType( itemTypeId ) == nullptr ) {
            qWarning() << "SkillFactory::createSkillCatalog"
                       << "Unknown ItemType, skipping SkillTree:" << itemTypeId;
            continue;
        }

        const Json::Value itemTypeJson = JsonHelper::loadJsonFile( ItemTypeFactory::typeFilePath( configPath, itemTypeId ) );

        if ( !itemTypeJson.isMember( "SkillTree" ) ) {
            continue;
        }

        SkillTreeModel skillTree;
        skillTree.setItemType( itemTypeId );

        const Json::Value& nodesJson = itemTypeJson[ "SkillTree" ];
        bool isTreeValid = true;

        for ( const Json::Value& nodeJson : nodesJson ) {

            if ( !nodeJson.isMember( "Type" ) || !nodeJson.isMember( "Name" ) ) {
                qWarning() << "SkillFactory::createSkillCatalog"
                           << "Invalid node, skipping SkillTree:" << itemTypeId;
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
    const QStringList keys = ItemTypeFactory::readTypeKeys( configPath );

    for ( const auto& treeEntry : skillCatalog.trees() ) {
        if ( !keys.contains( treeEntry.first ) ) {
            qWarning() << "SkillFactory::saveSkillCatalog"
                       << "Unknown ItemType, skipping SkillTree:" << treeEntry.first;
        }
    }

    for ( const QString& itemTypeId : keys ) {
        const QString file = ItemTypeFactory::typeFilePath( configPath, itemTypeId );

        if ( !QFile::exists( file ) ) {
            qWarning() << "SkillFactory::saveSkillCatalog"
                       << "ItemType file not found, skipping:" << file;
            continue;
        }

        Json::Value itemTypeJson = JsonHelper::loadJsonFile( file );
        const SkillTreeModel* skillTree = skillCatalog.tree( itemTypeId );

        if ( skillTree == nullptr ) {
            itemTypeJson.removeMember( "SkillTree" );

        } else {
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

        JsonHelper::saveJsonFile( file, itemTypeJson );
    }

    qInfo() << "SkillFactory::saveSkillCatalog";
}

} // namespace Engine
