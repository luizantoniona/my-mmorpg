#ifndef ENTITYTEXTUREMODEL_H
#define ENTITYTEXTUREMODEL_H

#include <QColor>
#include <QImage>

namespace Engine {

class EntityTextureModel {
public:
    static const QImage& characterTexture();
    static const QImage& creatureTexture();

private:
    static QImage createPlaceholder( const QColor& color );
};

} // namespace Engine

#endif // ENTITYTEXTUREMODEL_H
