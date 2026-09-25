#ifndef ITEMSLOTHELPER_H
#define ITEMSLOTHELPER_H

#include <string>

#include <MMORPGEngine/Data/Item/ItemSlotEnum.h>

namespace Engine {

class ItemSlotHelper {
public:
    static std::string toString( ItemSlotEnum slot );
    static ItemSlotEnum fromString( const std::string& value );
};

} // namespace Engine

#endif // ITEMSLOTHELPER_H
