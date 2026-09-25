#ifndef MONSTERMODEL_H
#define MONSTERMODEL_H

#include <cstdint>

#include <QString>

namespace Engine {

class MonsterModel {
public:
    MonsterModel();

    uint32_t type() const;
    void setType( uint32_t type );

    QString name() const;
    void setName( const QString& name );

private:
    QString _name;
    uint32_t _type;
};

} // namespace Engine

#endif // MONSTERMODEL_H
