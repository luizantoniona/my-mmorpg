#include "JsonHelper.h"

#include <QDebug>

#include <fstream>
#include <memory>
#include <sstream>

namespace Engine {

Json::Value JsonHelper::loadJsonFile( const QString& path ) {
    return loadJsonFile( path.toStdString() );
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

bool JsonHelper::saveJsonFile( const QString& path, const Json::Value& value ) {
    return saveJsonFile( path.toStdString(), value );
}

bool JsonHelper::saveJsonFile( const std::string& path, const Json::Value& value ) {
    std::ofstream file( path );
    if ( !file.is_open() ) {
        qWarning() << "JsonHelper::saveJsonFile Could not open file: " << QString::fromStdString( path );
        return false;
    }

    Json::StreamWriterBuilder builder;
    builder[ "indentation" ] = "    ";

    std::unique_ptr<Json::StreamWriter> writer( builder.newStreamWriter() );
    writer->write( value, &file );

    if ( file.fail() ) {
        qWarning() << "JsonHelper::saveJsonFile Could not write file:" << QString::fromStdString( path );
        return false;
    }

    return true;
}

Json::Value JsonHelper::parseJsonString( const QString& content ) {
    return parseJsonString( content.toStdString() );
}

Json::Value JsonHelper::parseJsonString( const std::string& content ) {
    Json::Value jsonData;
    Json::CharReaderBuilder builder;
    std::string errs;

    std::istringstream stream( content );
    if ( !Json::parseFromStream( builder, stream, &jsonData, &errs ) ) {
        qWarning() << "JsonHelper::parseJsonString Error parsing JSON:" << QString::fromStdString( errs );
        return Json::Value();
    }

    return jsonData;
}

std::string JsonHelper::writeJsonString( const Json::Value& value ) {
    Json::StreamWriterBuilder builder;
    builder[ "indentation" ] = "";

    return Json::writeString( builder, value );
}

} // namespace Engine
