#ifndef CHARACTERCATALOG_H
#define CHARACTERCATALOG_H

#include <map>
#include <vector>

#include <QSize>
#include <QString>

#include <MMORPGEngine/Data/Appearance/AppearanceDirectionEnum.h>
#include <MMORPGEngine/Data/Character/CharacterBodyModel.h>
#include <MMORPGEngine/Data/Character/CharacterOptionModel.h>
#include <MMORPGEngine/Entity/Character/CharacterAppearanceFeatureEnum.h>
#include <MMORPGEngine/Entity/Character/CharacterAppearanceModel.h>

namespace Engine {

class CharacterCatalog {
public:
    CharacterCatalog();

    const QSize& frameSize() const;
    void setFrameSize( const QSize& frameSize );

    const std::map<AppearanceDirectionEnum, int>& rows() const;
    void setRow( AppearanceDirectionEnum direction, int row );
    int rowCount() const;

    const CharacterBodyModel* body( const QString& id ) const;
    const std::map<QString, CharacterBodyModel>& bodies() const;
    void addBody( const CharacterBodyModel& body );

    std::vector<CharacterAppearanceFeatureEnum> features() const;
    std::vector<CharacterOptionModel> options( CharacterAppearanceFeatureEnum feature, const CharacterAppearanceModel& selection ) const;
    bool isRequired( CharacterAppearanceFeatureEnum feature ) const;
    bool isValidAppearance( const CharacterAppearanceModel& appearance ) const;

    bool isEmpty() const;

private:
    QSize _frameSize;
    std::map<AppearanceDirectionEnum, int> _rows;
    std::map<QString, CharacterBodyModel> _bodies;
};

} // namespace Engine

#endif // CHARACTERCATALOG_H
