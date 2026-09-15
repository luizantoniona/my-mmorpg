#ifndef SYNCPAGECONTROL_H
#define SYNCPAGECONTROL_H

#include <QList>
#include <QObject>

#include <MMORPGEngine/Data/Manifest/ManifestDataDTO.h>

class SyncPageControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( qreal progress READ progress NOTIFY progressChanged )

public:
    explicit SyncPageControl( QObject* parent = nullptr );
    ~SyncPageControl();

    qreal progress() const;

public slots:
    void sync();

signals:
    void syncUpdate( const QString& message );
    void syncSucceeded();
    void syncFailed( const QString& error );

    void progressChanged();

private:
    void downloadNext();

    void setProgress( qreal progress );

private:
    QList<Engine::ManifestDataDTO> _downloadQueue;
    int _downloadedCount;
    int _totalCount;
    qreal _progress;
};

#endif // SYNCPAGECONTROL_H
