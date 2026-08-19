#include "JsonHelper.h"

#include <QDebug>

#include <fstream>
#include <sstream>

namespace Engine {

Json::Value JsonHelper::loadJsonFile( const QString& path ) {
    std::ifstream file( path.toStdString() );
    if ( !file.is_open() ) {
        qWarning() << "JsonHelper::loadJsonFile Could not open file: " << path;
        return Json::Value();
    }

    Json::Value jsonData;
    file >> jsonData;
    return jsonData;
}

Json::Value JsonHelper::loadJsonFile( const std::string& path ) {
    std::ifstream file( path );
    if ( !file.is_open() ) {
        qWarning() << "JsonHelper::loadJsonFile Could not open file: " << path;
        return Json::Value();
    }

    Json::Value jsonData;
    file >> jsonData;
    return jsonData;
}

Json::Value JsonHelper::parseJsonString( const QString& content ) {
    Json::Value jsonData;
    Json::CharReaderBuilder builder;
    std::string errs;

    std::istringstream s( content.toStdString() );
    if ( !Json::parseFromStream( builder, s, &jsonData, &errs ) ) {
        qWarning() << "JsonHelper::parseJsonString Error parsing JSON: " << errs;
        return Json::Value();
    }

    return jsonData;
}

Json::Value JsonHelper::parseJsonString( const std::string& content ) {
    Json::Value jsonData;
    Json::CharReaderBuilder builder;
    std::string errs;

    std::istringstream s( content );
    if ( !Json::parseFromStream( builder, s, &jsonData, &errs ) ) {
        qWarning() << "JsonHelper::parseJsonString Error parsing JSON: " << errs;
        return Json::Value();
    }

    return jsonData;
}

} // namespace Engine
