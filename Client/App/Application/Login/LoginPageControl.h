#ifndef LOGINPAGECONTROL_H
#define LOGINPAGECONTROL_H

#include <QObject>

class LoginPageControl : public QObject {
    Q_OBJECT

public:
    explicit LoginPageControl( QObject* parent = nullptr );
    ~LoginPageControl();
};

#endif // LOGINPAGECONTROL_H
