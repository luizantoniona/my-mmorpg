#ifndef GROUNDITEMMODEL_H
#define GROUNDITEMMODEL_H

#include <QString>

#include <MMORPGEngine/Entity/EntityPositionModel.h>

namespace Engine {

class GroundItemModel {
public:
    GroundItemModel();

    int idGroundItem() const;
    void setIdGroundItem( int idGroundItem );

    QString idItem() const;
    void setIdItem( const QString& idItem );

    const EntityPositionModel& position() const;
    EntityPositionModel& position();

private:
    EntityPositionModel _position;
    QString _idItem;
    int _idGroundItem;
};

} // namespace Engine

#endif // GROUNDITEMMODEL_H
