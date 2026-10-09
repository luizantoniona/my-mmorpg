#ifndef CREATUREMODEL_H
#define CREATUREMODEL_H

#include <QString>

#include <MMORPGEngine/Entity/EntityModel.h>

namespace Engine {

class CreatureModel : public EntityModel {
public:
    CreatureModel();

    int idCreature() const;
    void setIdCreature( int idCreature );

    QString type() const;
    void setType( const QString& type );

private:
    QString _type;
    int _idCreature;
};

} // namespace Engine

#endif // CREATUREMODEL_H
