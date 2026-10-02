#ifndef ITEMICONPROVIDER_H
#define ITEMICONPROVIDER_H

#include <QQuickImageProvider>

class ItemIconProvider : public QQuickImageProvider {
public:
    ItemIconProvider();

    QImage requestImage( const QString& id, QSize* size, const QSize& requestedSize ) override;
};

#endif // ITEMICONPROVIDER_H
