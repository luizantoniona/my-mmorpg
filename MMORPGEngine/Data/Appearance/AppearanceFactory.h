#ifndef APPEARANCEFACTORY_H
#define APPEARANCEFACTORY_H

#include <QString>

#include <json/json.h>

#include <MMORPGEngine/Data/Appearance/AppearanceModel.h>

namespace Engine {

class AppearanceFactory {
public:
    static AppearanceModel createAppearance( const Json::Value& json, const QString& folderPath );
};

} // namespace Engine

#endif // APPEARANCEFACTORY_H
