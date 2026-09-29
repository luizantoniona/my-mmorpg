#ifndef GODOBSERVER_H
#define GODOBSERVER_H

#include <QObject>
#include <QString>

#include <MMORPGServer/Server/Network/Observer/EntityObserver.h>

namespace Server {

class GodObserver : public QObject, public EntityObserver {
    Q_OBJECT

public:
    explicit GodObserver( QObject* parent = nullptr );
    ~GodObserver() override;

    void send( const std::string& message ) override;

    void shutdown() override;

signals:
    void messageReceived( const QString& message );
};

} // namespace Server

#endif // GODOBSERVER_H
