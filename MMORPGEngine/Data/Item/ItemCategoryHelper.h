#ifndef ITEMCATEGORYHELPER_H
#define ITEMCATEGORYHELPER_H

#include <set>
#include <string>

#include <MMORPGEngine/Data/Item/ItemCategoryEnum.h>
#include <MMORPGEngine/Entity/CombatActionEnum.h>

namespace Engine {

class ItemCategoryHelper {
public:
    static std::string toString( ItemCategoryEnum category );
    static ItemCategoryEnum fromString( const std::string& value );

    static std::set<CombatActionEnum> grantedActions( ItemCategoryEnum category );
};

} // namespace Engine

#endif // ITEMCATEGORYHELPER_H
