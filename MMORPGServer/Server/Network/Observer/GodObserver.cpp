#include "GodObserver.h"

namespace Server {

GodObserver::GodObserver( QObject* parent ) :
    QObject( parent ) {
}

GodObserver::~GodObserver() = default;

void GodObserver::send( const std::string& message ) {
    const QString text = QString::fromStdString( message );

    QMetaObject::invokeMethod(
        this, [ this, text ]() {
            emit messageReceived( text );
        },
        Qt::QueuedConnection );
}

void GodObserver::shutdown() {
}

} // namespace Server
