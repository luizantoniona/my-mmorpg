#ifndef CREATUREMODEL_H
#define CREATUREMODEL_H

#include <MMORPGEngine/Entity/EntityModel.h>

namespace Engine {

class CreatureModel : public EntityModel {
public:
    CreatureModel();

    int idCreature() const;
    void setIdCreature( int idCreature );

private:
    int _idCreature;
};

} // namespace Engine

#endif // CREATUREMODEL_H
