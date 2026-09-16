#ifndef SERVERPAGECONTROL_H
#define SERVERPAGECONTROL_H

#include <QObject>
#include <QVariantList>

namespace Server {

class ServerPageControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( QString worldName READ worldName CONSTANT )
    Q_PROPERTY( int worldWidth READ worldWidth CONSTANT )
    Q_PROPERTY( int worldHeight READ worldHeight CONSTANT )
    Q_PROPERTY( QVariantList floors READ floors CONSTANT )

public:
    explicit ServerPageControl( QObject* parent = nullptr );

    QString worldName() const;
    int worldWidth() const;
    int worldHeight() const;
    QVariantList floors() const;

    Q_INVOKABLE QVariantList connectedCharacters() const;
    Q_INVOKABLE void disconnectCharacter( int idCharacter );
};

} // namespace Server

#endif // SERVERPAGECONTROL_H
