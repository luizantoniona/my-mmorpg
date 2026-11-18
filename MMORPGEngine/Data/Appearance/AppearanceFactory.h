#ifndef APPEARANCEFACTORY_H
#define APPEARANCEFACTORY_H

#include <QImage>
#include <QSize>
#include <QString>

#include <json/json.h>

#include <MMORPGEngine/Data/Animation/AnimationModel.h>
#include <MMORPGEngine/Data/Appearance/AppearanceModel.h>

namespace Engine {

class AppearanceFactory {
public:
    static AppearanceModel createAppearance( const Json::Value& json, const QString& folderPath );

    static AnimationModel createAnimation( const QImage& sheet, const QSize& frameSize, const QSize& stride, int row, int from, int count, int frameDurationMs );
};

} // namespace Engine

#endif // APPEARANCEFACTORY_H
