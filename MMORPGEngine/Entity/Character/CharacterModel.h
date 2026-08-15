#ifndef CHARACTERMODEL_H
#define CHARACTERMODEL_H

#include <string>

#include <MMORPGEngine/Entity/EntityModel.h>

namespace Engine {

class CharacterModel : public EntityModel {
public:
    CharacterModel();

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

    int idAccount() const;
    void setIdAccount( int idAccount );

    std::string name() const;
    void setName( const std::string& name );

private:
    int _idCharacter;
    int _idAccount;
    std::string _name;
};

} // namespace Engine

#endif // CHARACTERMODEL_H
