#ifndef AUTHFILTER_H
#define AUTHFILTER_H

#include <drogon/HttpFilter.h>

namespace Server {

class AuthFilter : public drogon::HttpFilter<AuthFilter> {
public:
    void doFilter( const drogon::HttpRequestPtr& request, drogon::FilterCallback&& callback, drogon::FilterChainCallback&& chainCallback ) override;
};

} // namespace Server

#endif // AUTHFILTER_H
