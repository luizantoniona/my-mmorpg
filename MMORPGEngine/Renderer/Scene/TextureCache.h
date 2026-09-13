#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include <QHash>
#include <QImage>

class QQuickWindow;
class QSGTexture;

namespace Engine {

class TextureCache {
public:
    TextureCache();
    ~TextureCache();

    TextureCache( const TextureCache& ) = delete;
    TextureCache& operator=( const TextureCache& ) = delete;

    QSGTexture* texture( QQuickWindow* window, const QImage& image );

private:
    void clear();

    QQuickWindow* _window;
    QHash<qint64, QSGTexture*> _textures;
};

} // namespace Engine

#endif // TEXTURECACHE_H
