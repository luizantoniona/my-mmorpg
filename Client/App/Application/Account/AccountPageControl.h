#ifndef ACCOUNTPAGECONTROL_H
#define ACCOUNTPAGECONTROL_H

#include <QObject>

class AccountPageControl : public QObject {
    Q_OBJECT

public:
    explicit AccountPageControl( QObject* parent = nullptr );
    ~AccountPageControl();
};

#endif // ACCOUNTPAGECONTROL_H
