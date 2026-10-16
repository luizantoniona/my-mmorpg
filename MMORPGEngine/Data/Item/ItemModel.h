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

    QString id() const;
    void setId( const QString& id );

    QString idItemType() const;
    void setIdItemType( const QString& idItemType );

    QString name() const;
    void setName( const QString& name );

    QString textureFile() const;
    void setTextureFile( const QString& textureFile );

    QImage texture() const;

    AnimationModel animation() const;
    void setAnimation( const AnimationModel& animation );

private:
    QString _id;
    QString _name;
    QString _idItemType;
    QString _textureFile;
    AnimationModel _animation;
};

} // namespace Engine

#endif // ITEMMODEL_H
