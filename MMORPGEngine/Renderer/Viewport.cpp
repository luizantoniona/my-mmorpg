#include "Viewport.h"

#include <QSGSimpleRectNode>

namespace Engine {

Viewport::Viewport( QQuickItem* parent ) :
    QQuickItem( parent ) {

    setFlag( ItemHasContents, true );
}

QSGNode* Viewport::updatePaintNode( QSGNode* oldNode, UpdatePaintNodeData* ) {
    auto* node = static_cast<QSGSimpleRectNode*>( oldNode );

    if ( !node ) {
        node = new QSGSimpleRectNode();
    }

    node->setRect( boundingRect() );

    node->setColor( Qt::red );

    return node;
}

} // namespace Engine
