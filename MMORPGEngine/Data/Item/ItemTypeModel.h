#ifndef ITEMTYPEMODEL_H
#define ITEMTYPEMODEL_H

#include <cstdint>

#include <QString>

namespace Engine {

class ItemTypeModel {
public:
    ItemTypeModel();

    uint32_t type() const;
    void setType( uint32_t type );

    QString name() const;
    void setName( const QString& name );

private:
    QString _name;
    uint32_t _type;
};

} // namespace Engine

#endif // ITEMTYPEMODEL_H
