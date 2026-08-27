#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <cstdint>

#include <QImage>
#include <QString>

#include <MMORPGEngine/Object/ObjectOrientationModel.h>
#include <MMORPGEngine/Object/ObjectPositionModel.h>
#include <MMORPGEngine/Object/ObjectSizeModel.h>

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

    ObjectOrientationModel orientation() const;
    void setOrientation( const ObjectOrientationModel& orientation );

    ObjectPositionModel position() const;
    void setPosition( const ObjectPositionModel& position );

    ObjectSizeModel size() const;
    void setSize( const ObjectSizeModel& size );

private:
    uint32_t _type;
    QString _name;
    QString _folder;
    QImage _texture;
    ObjectOrientationModel _orientation;
    ObjectPositionModel _position;
    ObjectSizeModel _size;
};

} // namespace Engine

#endif // OBJECTMODEL_H
