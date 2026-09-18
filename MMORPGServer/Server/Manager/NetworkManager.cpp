#include "NetworkManager.h"

#include <drogon/drogon.h>

namespace Server {

NetworkManager::NetworkManager() :
    _running( false ),
    _thread() {
}

NetworkManager::~NetworkManager() {
    finalize();
}

void NetworkManager::initialize() {
    if ( _running ) {
        return;
    }

    _running = true;

    _thread = std::thread( [ this ]() {
        drogon::app()
            .addListener( "0.0.0.0", 8080 )
            .setThreadNum( std::thread::hardware_concurrency() )
            .registerPostHandlingAdvice( []( const drogon::HttpRequestPtr& request, const drogon::HttpResponsePtr& response ) {
                response->addHeader( "Access-Control-Allow-Origin", "*" );
                response->addHeader( "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS" );
                response->addHeader( "Access-Control-Allow-Headers", "Content-Type, Authorization" );
            } )
            .run();
    } );
}

void NetworkManager::finalize() {
    if ( !_running ) {
        return;
    }

    _running = false;

    drogon::app().quit();

    if ( _thread.joinable() ) {
        _thread.join();
    }
}

} // namespace Server
