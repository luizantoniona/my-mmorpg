#include "TextureCache.h"

#include <QQuickWindow>
#include <QSGTexture>
#include <QtAlgorithms>

namespace Engine {

TextureCache::TextureCache() :
    _window( nullptr ) {
}

TextureCache::~TextureCache() {
    clear();
}

void TextureCache::clear() {
    qDeleteAll( _textures );
    _textures.clear();
}

QSGTexture* TextureCache::texture( QQuickWindow* window, const QImage& image ) {
    if ( window != _window ) {
        clear();
        _window = window;
    }

    const qint64 key = image.cacheKey();

    auto it = _textures.find( key );
    if ( it != _textures.end() ) {
        return it.value();
    }

    QSGTexture* texture = window->createTextureFromImage( image );
    if ( texture ) {
        _textures.insert( key, texture );
    }

    return texture;
}

} // namespace Engine
