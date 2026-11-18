#ifndef CHARACTERAPPEARANCEMODEL_H
#define CHARACTERAPPEARANCEMODEL_H

#include <map>

#include <QString>

#include <MMORPGEngine/Entity/Character/CharacterAppearanceFeatureEnum.h>

namespace Engine {

class CharacterAppearanceModel {
public:
    CharacterAppearanceModel();

    const std::map<CharacterAppearanceFeatureEnum, QString>& selections() const;

    bool has( CharacterAppearanceFeatureEnum feature ) const;
    QString id( CharacterAppearanceFeatureEnum feature ) const;
    void setId( CharacterAppearanceFeatureEnum feature, const QString& id );

    bool isEmpty() const;

    QString key() const;

private:
    std::map<CharacterAppearanceFeatureEnum, QString> _selections;
};

} // namespace Engine

#endif // CHARACTERAPPEARANCEMODEL_H
