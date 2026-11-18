#ifndef CHARACTERBODYMODEL_H
#define CHARACTERBODYMODEL_H

#include <map>

#include <QString>

#include <MMORPGEngine/Data/Appearance/AppearanceStateEnum.h>
#include <MMORPGEngine/Data/Character/CharacterClipModel.h>
#include <MMORPGEngine/Data/Character/CharacterSpriteModel.h>

namespace Engine {

class CharacterBodyModel {
public:
    CharacterBodyModel();

    const QString& id() const;
    void setId( const QString& id );

    const QString& name() const;
    void setName( const QString& name );

    const CharacterSpriteModel& sprite() const;
    CharacterSpriteModel& sprite();

    const std::map<AppearanceStateEnum, CharacterClipModel>& clips() const;
    const CharacterClipModel* clip( AppearanceStateEnum state ) const;
    void setClip( AppearanceStateEnum state, const CharacterClipModel& clip );

private:
    QString _id;
    QString _name;
    CharacterSpriteModel _sprite;
    std::map<AppearanceStateEnum, CharacterClipModel> _clips;
};

} // namespace Engine

#endif // CHARACTERBODYMODEL_H
