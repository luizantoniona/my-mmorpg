#ifndef CREATUREMODEL_H
#define CREATUREMODEL_H

#include <cstdint>

#include <MMORPGEngine/Entity/EntityModel.h>

namespace Engine {

class CreatureModel : public EntityModel {
public:
    CreatureModel();

    int idCreature() const;
    void setIdCreature( int idCreature );

    uint32_t type() const;
    void setType( uint32_t type );

private:
    int _idCreature;
    uint32_t _type;
};

} // namespace Engine

#endif // CREATUREMODEL_H
