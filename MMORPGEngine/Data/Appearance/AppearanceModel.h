#ifndef APPEARANCEMODEL_H
#define APPEARANCEMODEL_H

#include <map>
#include <utility>

#include <MMORPGEngine/Data/Animation/AnimationModel.h>
#include <MMORPGEngine/Data/Appearance/AppearanceDirectionEnum.h>
#include <MMORPGEngine/Data/Appearance/AppearanceStateEnum.h>

namespace Engine {

class AppearanceModel {
public:
    AppearanceModel();

    bool isNull() const;

    void setAnimation( AppearanceStateEnum state, AppearanceDirectionEnum direction, const AnimationModel& animation );

    const AnimationModel& animation( AppearanceStateEnum state, AppearanceDirectionEnum direction ) const;

private:
    const AnimationModel* find( AppearanceStateEnum state, AppearanceDirectionEnum direction ) const;

private:
    std::map<std::pair<AppearanceStateEnum, AppearanceDirectionEnum>, AnimationModel> _animations;
};

} // namespace Engine

#endif // APPEARANCEMODEL_H
