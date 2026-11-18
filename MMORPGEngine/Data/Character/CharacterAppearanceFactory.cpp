#include "CharacterAppearanceFactory.h"

#include <vector>

#include <QDebug>
#include <QImage>
#include <QPainter>

#include <MMORPGEngine/Data/Appearance/AppearanceFactory.h>
#include <MMORPGEngine/Data/Character/CharacterLayerHelper.h>

namespace Engine {

namespace {

QImage composeSheet( const std::vector<const CharacterSpriteModel*>& sprites, const CharacterCatalog& characterCatalog, AppearanceStateEnum state, const CharacterClipModel& clip ) {
    const QSize& frameSize = characterCatalog.frameSize();

    QImage sheet( QSize( frameSize.width() * clip.frameCount(), frameSize.height() * characterCatalog.rowCount() ), QImage::Format_ARGB32_Premultiplied );
    sheet.fill( Qt::transparent );

    QPainter painter( &sheet );
    bool hasBody = false;

    for ( const CharacterLayerEnum layer : CharacterLayerHelper::drawOrder() ) {
        for ( const CharacterSpriteModel* sprite : sprites ) {
            const QString file = sprite->file( layer, state );

            if ( file.isEmpty() ) {
                continue;
            }

            const QImage layerImage( sprite->folder() + file );

            if ( layerImage.isNull() ) {
                qWarning() << "CharacterAppearanceFactory::createAppearance"
                           << "Sheet not found, skipping" << sprite->folder() + file;
                continue;
            }

            painter.drawImage( 0, 0, layerImage );
            hasBody = hasBody || layer == CharacterLayerEnum::BODY;
        }
    }

    painter.end();

    return hasBody ? sheet : QImage();
}

} // namespace

AppearanceModel CharacterAppearanceFactory::createAppearance( const CharacterCatalog& characterCatalog, const CharacterAppearanceModel& characterAppearance ) {
    AppearanceModel appearance;

    const CharacterBodyModel* body = characterCatalog.body( characterAppearance.id( CharacterAppearanceFeatureEnum::BODY ) );

    if ( body == nullptr ) {
        return appearance;
    }

    const std::vector<const CharacterSpriteModel*> sprites{ &body->sprite() };

    for ( const auto& clipEntry : body->clips() ) {
        const CharacterClipModel& clip = clipEntry.second;

        const QImage sheet = composeSheet( sprites, characterCatalog, clipEntry.first, clip );

        if ( sheet.isNull() ) {
            continue;
        }

        for ( const auto& rowEntry : characterCatalog.rows() ) {
            const AnimationModel animation = AppearanceFactory::createAnimation( sheet, characterCatalog.frameSize(), characterCatalog.frameSize(), rowEntry.second, 0, clip.frameCount(), clip.frameDurationMs() );

            if ( !animation.isNull() ) {
                appearance.setAnimation( clipEntry.first, rowEntry.first, animation );
            }
        }
    }

    return appearance;
}

} // namespace Engine
