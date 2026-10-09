#ifndef ENTITYTEXTUREMODEL_H
#define ENTITYTEXTUREMODEL_H

#include <QColor>
#include <QImage>

namespace Engine {

class EntityTextureModel {
public:
    static const QImage& characterTexture();
    static const QImage& creatureTexture( int footprint = 1 );

private:
    static QImage createPlaceholder( const QColor& color, int footprint = 1 );
};

} // namespace Engine

#endif // ENTITYTEXTUREMODEL_H
