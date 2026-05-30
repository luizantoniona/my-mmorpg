#ifndef USERMODEL_H
#define USERMODEL_H

#include <string>

namespace Engine {

class UserModel {
public:
    UserModel();

    int idUser() const;
    void setIdUser( int idUser );

    std::string dsUsername() const;
    void setDsUsername( const std::string& dsUsername );

    std::string dsPassword() const;
    void setDsPassword( const std::string& dsPassword );

private:
    int _idUser;
    std::string _dsUsername;
    std::string _dsPassword;
};

} // namespace Engine

#endif // USERMODEL_H
