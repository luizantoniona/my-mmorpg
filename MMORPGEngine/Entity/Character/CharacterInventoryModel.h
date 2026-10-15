#ifndef CHARACTERINVENTORYMODEL_H
#define CHARACTERINVENTORYMODEL_H

#include <cstdint>

#include <QString>

namespace Engine {

class CharacterInventoryModel {
public:
    CharacterInventoryModel();

    uint32_t position() const;
    void setPosition( uint32_t position );

    QString idItem() const;
    void setIdItem( const QString& idItem );

    uint32_t amount() const;
    void setAmount( uint32_t amount );

private:
    QString _idItem;
    uint32_t _position;
    uint32_t _amount;
};

} // namespace Engine

#endif // CHARACTERINVENTORYMODEL_H
