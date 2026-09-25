#ifndef CHARACTERPROFICIENCYMODEL_H
#define CHARACTERPROFICIENCYMODEL_H

#include <cstdint>

namespace Engine {

class CharacterProficiencyModel {
public:
    CharacterProficiencyModel();

    uint32_t idItemType() const;
    void setIdItemType( uint32_t idItemType );

    double xp() const;
    void setXp( double xp );

    uint32_t lvl() const;
    void setLvl( uint32_t lvl );

private:
    double _xp;
    uint32_t _idItemType;
    uint32_t _lvl;
};

} // namespace Engine

#endif // CHARACTERPROFICIENCYMODEL_H
