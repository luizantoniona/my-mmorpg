#ifndef CONFIGURATIONCONTROL_H
#define CONFIGURATIONCONTROL_H

#include <QObject>
#include <QString>
#include <QStringList>

class ConfigurationControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( QStringList mapFolders READ mapFolders NOTIFY mapFoldersChanged )
    Q_PROPERTY( QString activeFolder READ activeFolder NOTIFY activeFolderChanged )
    Q_PROPERTY( QString mapName READ mapName NOTIFY activeFolderChanged )
    Q_PROPERTY( int mapWidth READ mapWidth NOTIFY activeFolderChanged )
    Q_PROPERTY( int mapHeight READ mapHeight NOTIFY activeFolderChanged )
    Q_PROPERTY( int floorCount READ floorCount NOTIFY activeFolderChanged )
    Q_PROPERTY( QString lastError READ lastError NOTIFY lastErrorChanged )

public:
    explicit ConfigurationControl( QObject* parent = nullptr );

    QStringList mapFolders() const;
    QString activeFolder() const;
    QString mapName() const;
    int mapWidth() const;
    int mapHeight() const;
    int floorCount() const;
    QString lastError() const;

public slots:
    bool selectFolder( const QString& folder );
    bool resizeMap( int width, int height );

signals:
    void mapFoldersChanged();
    void activeFolderChanged();
    void lastErrorChanged();

private:
    void reloadMapFolders();
    void reloadActiveMap();
    void setLastError( const QString& error );

    QStringList _mapFolders;
    QString _activeFolder;
    QString _mapName;
    int _mapWidth;
    int _mapHeight;
    int _floorCount;
    QString _lastError;
};

#endif // CONFIGURATIONCONTROL_H
