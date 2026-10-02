#ifndef CHARACTERINVENTORYMODEL_H
#define CHARACTERINVENTORYMODEL_H

#include <cstdint>

namespace Engine {

class CharacterInventoryModel {
public:
    CharacterInventoryModel();

    uint32_t position() const;
    void setPosition( uint32_t position );

    uint32_t idItem() const;
    void setIdItem( uint32_t idItem );

    uint32_t amount() const;
    void setAmount( uint32_t amount );

private:
    uint32_t _position;
    uint32_t _idItem;
    uint32_t _amount;
};

} // namespace Engine

#endif // CHARACTERINVENTORYMODEL_H
