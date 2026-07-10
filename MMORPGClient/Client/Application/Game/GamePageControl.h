#ifndef GAMEPAGECONTROL_H
#define GAMEPAGECONTROL_H

#include <QObject>

class GamePageControl : public QObject {
    Q_OBJECT

public:
    explicit GamePageControl( QObject* parent = nullptr );
    ~GamePageControl();
};

#endif // GAMEPAGECONTROL_H
