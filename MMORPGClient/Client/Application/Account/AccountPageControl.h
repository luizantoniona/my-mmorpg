#ifndef ACCOUNTPAGECONTROL_H
#define ACCOUNTPAGECONTROL_H

#include <QObject>

class AccountPageControl : public QObject {
    Q_OBJECT

public:
    explicit AccountPageControl( QObject* parent = nullptr );
    ~AccountPageControl();

public slots:
    void logout();

    void removeCharacter( int idCharacter );

signals:
    void logoutSucceeded();
    void logoutFailed( const QString& error );

    void characterRemoved();
    void characterRemovalFailed( const QString& error );
};

#endif // ACCOUNTPAGECONTROL_H
