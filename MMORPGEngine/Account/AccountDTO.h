#ifndef ACCOUNTDTO_H
#define ACCOUNTDTO_H

#include <string>

#include <json/json.h>

#include "AccountModel.h"

namespace Engine {

class AccountDTO {
public:
    AccountDTO();
    explicit AccountDTO( const AccountModel& account, const std::string& sessionId );
    ~AccountDTO();

    Json::Value toJson() const;
    static AccountDTO fromJson( const Json::Value& json );

    int idAccount() const;
    void setIdAccount( int idAccount );

    std::string username() const;
    void setUsername( const std::string& username );

    std::string sessionId() const;
    void setSessionId( const std::string& sessionId );

private:
    int _idAccount;
    std::string _username;
    std::string _sessionId;
};

} // namespace Engine

#endif // ACCOUNTDTO_H
