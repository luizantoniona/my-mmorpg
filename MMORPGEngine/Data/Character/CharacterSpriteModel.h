#ifndef CHARACTERSPRITEMODEL_H
#define CHARACTERSPRITEMODEL_H

#include <map>

#include <QString>

#include <MMORPGEngine/Data/Appearance/AppearanceStateEnum.h>
#include <MMORPGEngine/Data/Character/CharacterLayerEnum.h>

namespace Engine {

class CharacterSpriteModel {
public:
    CharacterSpriteModel();

    const QString& folder() const;
    void setFolder( const QString& folder );

    QString file( CharacterLayerEnum layer, AppearanceStateEnum state ) const;
    void setFile( CharacterLayerEnum layer, AppearanceStateEnum state, const QString& file );

    bool hasLayer( CharacterLayerEnum layer ) const;

private:
    QString _folder;
    std::map<CharacterLayerEnum, std::map<AppearanceStateEnum, QString>> _files;
};

} // namespace Engine

#endif // CHARACTERSPRITEMODEL_H
