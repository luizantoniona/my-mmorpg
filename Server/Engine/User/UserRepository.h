#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <memory>
#include <string>

#include <Engine/Database/Repository.h>

#include "UserModel.h"

namespace Engine {

class UserRepository : public Repository {
public:
    explicit UserRepository();

    bool createUser( std::string& username, std::string& password );

    std::unique_ptr<UserModel> findByUsername( const std::string& username );
    std::unique_ptr<UserModel> findByUsernameAndPassword( const std::string& username, const std::string& password );
};

} // namespace Engine

#endif // USERREPOSITORY_H
