#include "Renderer.h"

#include <QColor>
#include <QPainter>

#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

namespace {
QImage createEntityPlaceholder() {
    const int size = WorldConstants::TILE_SIZE;

    QImage image( size, size, QImage::Format_ARGB32_Premultiplied );
    image.fill( Qt::transparent );

    QPainter painter( &image );
    painter.setRenderHint( QPainter::Antialiasing );
    painter.setPen( Qt::NoPen );
    painter.setBrush( QColor( 66, 135, 245 ) );
    painter.drawEllipse( image.rect().adjusted( 4, 4, -4, -4 ) );

    return image;
}
} // namespace

Renderer::Renderer() :
    _viewportSize( 0.0, 0.0 ),
    _entityTexture( createEntityPlaceholder() ),
    _entityRenderer( new EntityRenderer() ),
    _objectRenderer( new ObjectRenderer() ),
    _tileRenderer( new TileRenderer() ) {
}

void Renderer::initialize() {
}

void Renderer::resize( const QSizeF& size ) {
    _viewportSize = size;
}

void Renderer::render( RenderScene& scene, const Camera& camera, const RenderWorld& world, int z ) {
    // TODO: Order is important here, see if we can structure it in a better way
    _tileRenderer->render( scene, camera, world, z );
    _objectRenderer->render( scene, camera, world, z );
    _entityRenderer->render( scene, camera, world, z, _entityTexture );
}

} // namespace Engine
