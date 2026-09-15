#ifndef MANIFESTFACTORY_H
#define MANIFESTFACTORY_H

#include <string>

#include <QString>

#include <MMORPGEngine/Data/Manifest/ManifestModel.h>

namespace Engine {

class ManifestFactory {
public:
    static ManifestModel createManifest( const std::string& configPath );

    static std::string hashFile( const QString& path );
};

} // namespace Engine

#endif // MANIFESTFACTORY_H
