#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <cstdint>

#include <QImage>
#include <QString>

#include <MMORPGEngine/Data/Animation/AnimationModel.h>

namespace Engine {

class ItemModel {
public:
    ItemModel();

    uint32_t id() const;
    void setId( uint32_t id );

    QString idItemType() const;
    void setIdItemType( const QString& idItemType );

    QString name() const;
    void setName( const QString& name );

    QString folder() const;
    void setFolder( const QString& folder );

    QImage texture() const;

    AnimationModel animation() const;
    void setAnimation( const AnimationModel& animation );

private:
    QString _name;
    QString _idItemType;
    QString _folder;
    AnimationModel _animation;
    uint32_t _id;
};

} // namespace Engine

#endif // ITEMMODEL_H
