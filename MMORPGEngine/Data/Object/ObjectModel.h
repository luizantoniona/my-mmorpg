#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <cstdint>
#include <optional>

#include <QImage>
#include <QString>

#include <MMORPGEngine/Data/Animation/AnimationModel.h>
#include <MMORPGEngine/Data/Object/ObjectInteractionModel.h>
#include <MMORPGEngine/Data/Object/ObjectSizeModel.h>

namespace Engine {

class ObjectModel {
public:
    ObjectModel();
    ~ObjectModel();

    uint32_t type() const;
    void setType( uint32_t type );

    QString name() const;
    void setName( const QString& name );

    QString folder() const;
    void setFolder( const QString& folder );

    QImage texture() const;
    void setTexture( const QImage& texture );

    AnimationModel animation() const;
    void setAnimation( const AnimationModel& animation );

    ObjectSizeModel size() const;
    void setSize( const ObjectSizeModel& size );

    const std::optional<ObjectInteractionModel>& interaction() const;
    void setInteraction( const ObjectInteractionModel& interaction );

private:
    uint32_t _type;
    QString _name;
    QString _folder;
    AnimationModel _animation;
    ObjectSizeModel _size;
    std::optional<ObjectInteractionModel> _interaction;
};

} // namespace Engine

#endif // OBJECTMODEL_H
