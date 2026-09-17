#ifndef TILECREATIONCONTROL_H
#define TILECREATIONCONTROL_H

#include <QObject>
#include <QString>

class TileCreationControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( int nextType READ nextType NOTIFY catalogChanged )
    Q_PROPERTY( QString lastError READ lastError NOTIFY lastErrorChanged )

public:
    explicit TileCreationControl( QObject* parent = nullptr );

    int nextType() const;
    QString lastError() const;

public slots:
    bool createTile( const QString& name, const QString& textureFile, const QString& tagsText, int frameDurationMs, bool isWalkable );

signals:
    void catalogChanged();
    void lastErrorChanged();

private:
    void setLastError( const QString& error );

    QString _lastError;
};

#endif // TILECREATIONCONTROL_H
