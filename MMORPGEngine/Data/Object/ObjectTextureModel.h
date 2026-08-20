#ifndef OBJECTTEXTUREMODEL_H
#define OBJECTTEXTUREMODEL_H

#include <cstdint>

#include <QImage>
#include <QString>

namespace Engine {

class ObjectTextureModel {
public:
    ObjectTextureModel();

    uint32_t type() const;
    void setType( uint32_t type );

    QString name() const;
    void setName( const QString& name );

    QString folder() const;
    void setFolder( const QString& folder );

    QImage texture() const;
    void setTexture( const QImage& texture );

private:
    uint32_t _type;
    QString _name;
    QString _folder;
    QImage _texture;
};

} // namespace Engine

#endif // OBJECTTEXTUREMODEL_H
