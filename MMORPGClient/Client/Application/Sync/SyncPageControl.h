#ifndef SYNCPAGECONTROL_H
#define SYNCPAGECONTROL_H

#include <QObject>

class SyncPageControl : public QObject {
    Q_OBJECT

public:
    explicit SyncPageControl( QObject* parent = nullptr );
    ~SyncPageControl();

public slots:
    void sync();

signals:
    void syncUpdate( const QString& message );
    void syncSucceeded();
    void syncFailed( const QString& error );
};

#endif // SYNCPAGECONTROL_H
