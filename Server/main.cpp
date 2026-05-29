#include <drogon/drogon.h>

namespace {

} // namespace

int main() {
    std::cout << "Starting Server" << std::endl;

    drogon::app()
        .addListener( "0.0.0.0", 8080 )
        .setThreadNum( std::thread::hardware_concurrency() )
        .registerPostHandlingAdvice( []( const drogon::HttpRequestPtr& request, const drogon::HttpResponsePtr& response ) {
            response->addHeader( "Access-Control-Allow-Origin", "*" );
            response->addHeader( "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS" );
            response->addHeader( "Access-Control-Allow-Headers", "Content-Type, Authorization" );
        } )
        .run();

    std::cout << "Ending Server" << std::endl;

    return 0;
}
