#ifndef ACCOUNTDTO_H
#define ACCOUNTDTO_H

#include <string>

#include <json/json.h>

#include "AccountModel.h"

namespace Engine {

class AccountDTO {
public:
    AccountDTO();
    explicit AccountDTO( const AccountModel& account );

    Json::Value toJson() const;

    int idAccount() const;
    void setIdAccount( int idAccount );

    std::string username() const;
    void setUsername( const std::string& username );

private:
    int _idAccount;
    std::string _username;
};

} // namespace Engine

#endif // ACCOUNTDTO_H
