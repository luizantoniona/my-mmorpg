#ifndef CHARACTERATTACKRESOLVER_H
#define CHARACTERATTACKRESOLVER_H

#include <vector>

#include <MMORPGEngine/Data/Attack/AttackModel.h>
#include <MMORPGEngine/Entity/Character/CharacterEquipmentModel.h>

namespace Server {

class CharacterAttackResolver {
public:
    static Engine::AttackModel attack( const std::vector<Engine::CharacterEquipmentModel>& equipment );
    static double damage( const std::vector<Engine::CharacterEquipmentModel>& equipment );
};

} // namespace Server

#endif // CHARACTERATTACKRESOLVER_H
