#ifndef GROUNDMODEL_H
#define GROUNDMODEL_H

#include <cstdint>

#include <QString>

namespace Engine {

class GroundModel {
public:
    GroundModel();

    uint16_t type() const;
    void setType( uint16_t type );

    bool walkable() const;
    void setWalkable( bool walkable );

    QString name() const;
    void setName( const QString& name );

    QString folder() const;
    void setFolder( const QString& folder );

private:
    uint16_t _type;
    bool _walkable;
    QString _name;
    QString _folder;
};

} // namespace Engine

#endif // GROUNDMODEL_H
