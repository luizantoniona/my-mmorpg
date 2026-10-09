#ifndef CHARACTERCOMBATACTIONRESOLVER_H
#define CHARACTERCOMBATACTIONRESOLVER_H

#include <set>
#include <vector>

#include <MMORPGEngine/Entity/Character/CharacterEquipmentModel.h>
#include <MMORPGEngine/Entity/CombatActionEnum.h>

namespace Server {

class CharacterCombatActionResolver {
public:
    static std::set<Engine::CombatActionEnum> resolve( const std::vector<Engine::CharacterEquipmentModel>& equipment );
};

} // namespace Server

#endif // CHARACTERCOMBATACTIONRESOLVER_H
