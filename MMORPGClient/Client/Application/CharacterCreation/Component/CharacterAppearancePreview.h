#ifndef CHARACTERAPPEARANCEPREVIEW_H
#define CHARACTERAPPEARANCEPREVIEW_H

#include <QElapsedTimer>
#include <QQuickPaintedItem>
#include <QTimer>
#include <QVariantMap>

#include <MMORPGEngine/Data/Appearance/AppearanceModel.h>

class CharacterAppearancePreview : public QQuickPaintedItem {
    Q_OBJECT

    Q_PROPERTY( QVariantMap appearance READ appearance WRITE setAppearance NOTIFY appearanceChanged )

public:
    explicit CharacterAppearancePreview( QQuickItem* parent = nullptr );
    ~CharacterAppearancePreview();

    const QVariantMap& appearance() const;
    void setAppearance( const QVariantMap& appearance );

    void paint( QPainter* painter ) override;

signals:
    void appearanceChanged();

private:
    Engine::AppearanceDirectionEnum currentDirection() const;

private:
    Engine::AppearanceModel _composedAppearance;
    QTimer _repaintTimer;
    QElapsedTimer _elapsed;
    QVariantMap _appearance;
};

#endif // CHARACTERAPPEARANCEPREVIEW_H
