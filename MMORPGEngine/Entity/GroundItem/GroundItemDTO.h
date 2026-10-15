#ifndef GROUNDITEMDTO_H
#define GROUNDITEMDTO_H

#include <json/json.h>

#include <QString>

#include <MMORPGEngine/Entity/GroundItem/GroundItemModel.h>

namespace Engine {

class GroundItemDTO {
public:
    GroundItemDTO();
    ~GroundItemDTO();

    static GroundItemDTO fromModel( const GroundItemModel* groundItem );
    static GroundItemDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idGroundItem() const;
    void setIdGroundItem( int idGroundItem );

    QString idItem() const;
    void setIdItem( const QString& idItem );

    int x() const;
    void setX( int x );

    int y() const;
    void setY( int y );

    int z() const;
    void setZ( int z );

private:
    QString _idItem;
    int _idGroundItem;
    int _x;
    int _y;
    int _z;
};

} // namespace Engine

#endif // GROUNDITEMDTO_H
