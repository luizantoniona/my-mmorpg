#ifndef ITEMCATEGORYHELPER_H
#define ITEMCATEGORYHELPER_H

#include <string>

#include <MMORPGEngine/Data/Item/ItemCategoryEnum.h>

namespace Engine {

class ItemCategoryHelper {
public:
    static std::string toString( ItemCategoryEnum category );
    static ItemCategoryEnum fromString( const std::string& value );
};

} // namespace Engine

#endif // ITEMCATEGORYHELPER_H
