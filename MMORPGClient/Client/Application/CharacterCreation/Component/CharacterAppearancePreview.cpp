#include "CharacterAppearancePreview.h"

#include <QPainter>

#include <MMORPGClient/Client/Application/CharacterCreation/CharacterCreationPageControl.h>
#include <MMORPGEngine/Commons/Singleton.h>
#include <MMORPGEngine/Data/Character/CharacterAppearanceFactory.h>
#include <MMORPGEngine/Data/DataManager.h>

namespace {

constexpr int REPAINT_INTERVAL_MS = 50;
constexpr qint64 DIRECTION_DURATION_MS = 2000;
constexpr qint64 DIRECTION_COUNT = 4;

} // namespace

CharacterAppearancePreview::CharacterAppearancePreview( QQuickItem* parent ) :
    QQuickPaintedItem( parent ),
    _composedAppearance(),
    _repaintTimer(),
    _elapsed(),
    _appearance() {
    setAntialiasing( false );

    _elapsed.start();

    _repaintTimer.setInterval( REPAINT_INTERVAL_MS );
    connect( &_repaintTimer, &QTimer::timeout, this, [ this ]() { update(); } );
    _repaintTimer.start();
}

CharacterAppearancePreview::~CharacterAppearancePreview() = default;

const QVariantMap& CharacterAppearancePreview::appearance() const {
    return _appearance;
}

void CharacterAppearancePreview::setAppearance( const QVariantMap& appearance ) {
    if ( _appearance == appearance ) {
        return;
    }

    _appearance = appearance;

    const Engine::CharacterCatalog& characterCatalog = Engine::Singleton<Engine::DataManager>::instance().characterCatalog();
    const Engine::CharacterAppearanceModel selection = CharacterCreationPageControl::appearanceFromSelections( _appearance );

    _composedAppearance = characterCatalog.isValidAppearance( selection ) ? Engine::CharacterAppearanceFactory::createAppearance( characterCatalog, selection ) : Engine::AppearanceModel();
    _elapsed.restart();

    emit appearanceChanged();
    update();
}

void CharacterAppearancePreview::paint( QPainter* painter ) {
    if ( _composedAppearance.isNull() ) {
        return;
    }

    const QImage frame = _composedAppearance.animation( Engine::AppearanceStateEnum::WALK, currentDirection() ).frameAt( _elapsed.elapsed() );

    if ( frame.isNull() ) {
        return;
    }

    const QSize scaledSize = frame.size().scaled( size().toSize(), Qt::KeepAspectRatio );
    const QRect target( ( width() - scaledSize.width() ) / 2, ( height() - scaledSize.height() ) / 2, scaledSize.width(), scaledSize.height() );

    painter->setRenderHint( QPainter::SmoothPixmapTransform, false );
    painter->drawImage( target, frame );
}

Engine::AppearanceDirectionEnum CharacterAppearancePreview::currentDirection() const {
    switch ( ( _elapsed.elapsed() / DIRECTION_DURATION_MS ) % DIRECTION_COUNT ) {
    case 1:
        return Engine::AppearanceDirectionEnum::WEST;
    case 2:
        return Engine::AppearanceDirectionEnum::NORTH;
    case 3:
        return Engine::AppearanceDirectionEnum::EAST;
    default:
        return Engine::AppearanceDirectionEnum::SOUTH;
    }
}
