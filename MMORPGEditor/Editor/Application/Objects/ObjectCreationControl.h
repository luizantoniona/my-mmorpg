#ifndef OBJECTCREATIONCONTROL_H
#define OBJECTCREATIONCONTROL_H

#include <QObject>
#include <QString>

class ObjectCreationControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( int nextType READ nextType NOTIFY catalogChanged )
    Q_PROPERTY( QString lastError READ lastError NOTIFY lastErrorChanged )

public:
    explicit ObjectCreationControl( QObject* parent = nullptr );

    int nextType() const;
    QString lastError() const;

public slots:
    bool createObject( const QString& name, const QString& textureFile, int width, int height, int frameDurationMs );

signals:
    void catalogChanged();
    void lastErrorChanged();

private:
    void setLastError( const QString& error );

    QString _lastError;
};

#endif // OBJECTCREATIONCONTROL_H
