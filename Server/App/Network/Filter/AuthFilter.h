#ifndef AUTHFILTER_H
#define AUTHFILTER_H

#include <drogon/HttpFilter.h>

namespace Engine {

class AuthFilter : public drogon::HttpFilter<AuthFilter> {
public:
    void doFilter( const drogon::HttpRequestPtr& request, drogon::FilterCallback&& callback, drogon::FilterChainCallback&& chainCallback ) override;
};

} // namespace Engine

#endif // AUTHFILTER_H
