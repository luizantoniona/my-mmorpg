#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <initializer_list>
#include <string>

#include <json/json.h>

#include <QString>

#include <MMORPGEngine/Commons/JsonFieldRule.h>

namespace Engine {

class JsonHelper {
public:
    static Json::Value loadJsonFile( const QString& path );
    static Json::Value loadJsonFile( const std::string& path );

    static bool saveJsonFile( const QString& path, const Json::Value& value );
    static bool saveJsonFile( const std::string& path, const Json::Value& value );

    static Json::Value parseJsonString( const QString& content );
    static Json::Value parseJsonString( const std::string& content );

    static std::string writeJsonString( const Json::Value& value );

    static bool hasValidFields( const Json::Value& json, std::initializer_list<JsonFieldRule> rules );
};

} // namespace Engine

#endif // JSONHELPER_H
