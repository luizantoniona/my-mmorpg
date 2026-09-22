#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <cstdint>

#include <QString>

namespace Engine {

class ItemModel {
public:
    ItemModel();

    uint32_t id() const;
    void setId( uint32_t id );

    uint32_t idItemType() const;
    void setIdItemType( uint32_t idItemType );

    QString name() const;
    void setName( const QString& name );

private:
    QString _name;
    uint32_t _id;
    uint32_t _idItemType;
};

} // namespace Engine

#endif // ITEMMODEL_H
