#ifndef SERVERPAGECONTROL_H
#define SERVERPAGECONTROL_H

#include <QObject>

namespace Server {

class ServerPageControl : public QObject {
    Q_OBJECT

public:
    explicit ServerPageControl( QObject* parent = nullptr );
};

} // namespace Server

#endif // SERVERPAGECONTROL_H
