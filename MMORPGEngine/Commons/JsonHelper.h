#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <string>

#include <json/json.h>

#include <QString>

namespace Engine {

class JsonHelper {
public:
    static Json::Value loadJsonFile( const QString& path );
    static Json::Value loadJsonFile( const std::string& path );

    static Json::Value parseJsonString( const QString& content );
    static Json::Value parseJsonString( const std::string& content );
};

} // namespace Engine

#endif // JSONHELPER_H
