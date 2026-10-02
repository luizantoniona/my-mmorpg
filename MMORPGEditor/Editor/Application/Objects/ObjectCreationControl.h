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

    Q_INVOKABLE int nextFreeType( int after ) const;
    Q_INVOKABLE QString typeName( int type ) const;

public slots:
    bool createObject( int type, const QString& name, const QString& textureFile, int width, int height, int frameDurationMs, bool replace );

signals:
    void catalogChanged();
    void lastErrorChanged();

private:
    void setLastError( const QString& error );

    QString _lastError;
};

#endif // OBJECTCREATIONCONTROL_H
