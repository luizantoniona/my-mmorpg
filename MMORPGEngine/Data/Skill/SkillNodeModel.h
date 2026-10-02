#ifndef SKILLNODEMODEL_H
#define SKILLNODEMODEL_H

#include <cstdint>

#include <QList>
#include <QString>

namespace Engine {

class SkillNodeModel {
public:
    SkillNodeModel();

    uint32_t type() const;
    void setType( uint32_t type );

    QString name() const;
    void setName( const QString& name );

    uint32_t proficiencyLevel() const;
    void setProficiencyLevel( uint32_t proficiencyLevel );

    QList<uint32_t> prerequisites() const;
    void setPrerequisites( const QList<uint32_t>& prerequisites );

private:
    QString _name;
    QList<uint32_t> _prerequisites;
    uint32_t _type;
    uint32_t _proficiencyLevel;
};

} // namespace Engine

#endif // SKILLNODEMODEL_H
