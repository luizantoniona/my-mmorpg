#ifndef GROUNDMODEL_H
#define GROUNDMODEL_H

#include <cstdint>

#include <QString>

namespace Engine {

class GroundModel {
public:
    GroundModel();

    uint32_t type() const;
    void setType( uint32_t type );

    QString name() const;
    void setName( const QString& name );

    QString folder() const;
    void setFolder( const QString& folder );

private:
    uint32_t _type;
    QString _name;
    QString _folder;
};

} // namespace Engine

#endif // GROUNDMODEL_H
