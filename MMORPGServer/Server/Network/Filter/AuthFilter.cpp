#include "AuthFilter.h"

#include <MMORPGEngine/Commons/Singleton.h>
#include <Network/NetworkServer.h>
#include <Network/NetworkSession.h>

namespace {
constexpr const char* AUTH_PREFIX = "X-Session ";
constexpr const char* SESSION_ATTRIBUTE = "Session";
} // namespace

namespace Server {

void AuthFilter::doFilter( const drogon::HttpRequestPtr& request, drogon::FilterCallback&& callback, drogon::FilterChainCallback&& chainCallback ) {
    if ( request->method() == drogon::HttpMethod::Options ) {
        chainCallback();
        return;
    }

    auto token = request->getHeader( "Authorization" );

    if ( token.rfind( AUTH_PREFIX, 0 ) == 0 ) {
        std::string sessionId = token.substr( std::strlen( AUTH_PREFIX ) );

        std::optional<NetworkSession> session = Engine::Singleton<NetworkServer>::instance().getSession( sessionId );

        if ( session ) {
            request->attributes()->insert( SESSION_ATTRIBUTE, std::make_shared<NetworkSession>( *session ) );
            chainCallback();
            return;
        }
    }

    auto response = drogon::HttpResponse::newHttpResponse();
    response->setStatusCode( drogon::k401Unauthorized );
    response->setBody( "Unauthorized" );
    callback( response );
}

const NetworkSession& AuthFilter::session( const drogon::HttpRequestPtr& request ) {
    auto session = request->attributes()->get<std::shared_ptr<NetworkSession>>( SESSION_ATTRIBUTE );
    return *session;
}

} // namespace Server
