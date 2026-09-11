#ifndef TILEICONPROVIDER_H
#define TILEICONPROVIDER_H

#include <QQuickImageProvider>

class TileIconProvider : public QQuickImageProvider {
public:
    TileIconProvider();

    QImage requestImage( const QString& id, QSize* size, const QSize& requestedSize ) override;
};

#endif // TILEICONPROVIDER_H
