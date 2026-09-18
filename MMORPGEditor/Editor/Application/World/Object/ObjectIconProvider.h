#ifndef OBJECTICONPROVIDER_H
#define OBJECTICONPROVIDER_H

#include <QQuickImageProvider>

class ObjectIconProvider : public QQuickImageProvider {
public:
    ObjectIconProvider();

    QImage requestImage( const QString& id, QSize* size, const QSize& requestedSize ) override;
};

#endif // OBJECTICONPROVIDER_H
