#ifndef CHARACTERPROFICIENCYMODEL_H
#define CHARACTERPROFICIENCYMODEL_H

#include <cstdint>

#include <QString>

namespace Engine {

class CharacterProficiencyModel {
public:
    CharacterProficiencyModel();

    QString idItemType() const;
    void setIdItemType( const QString& idItemType );

    double xp() const;
    void setXp( double xp );

    uint32_t lvl() const;
    void setLvl( uint32_t lvl );

private:
    double _xp;
    QString _idItemType;
    uint32_t _lvl;
};

} // namespace Engine

#endif // CHARACTERPROFICIENCYMODEL_H
