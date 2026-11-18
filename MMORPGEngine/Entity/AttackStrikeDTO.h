#ifndef ATTACKSTRIKEDTO_H
#define ATTACKSTRIKEDTO_H

#include <utility>
#include <vector>

#include <json/json.h>

namespace Engine {

class AttackStrikeDTO {
public:
    AttackStrikeDTO();
    AttackStrikeDTO( double seconds, std::vector<std::pair<int, int>> cells );

    static AttackStrikeDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    double seconds() const;

    const std::vector<std::pair<int, int>>& cells() const;

private:
    std::vector<std::pair<int, int>> _cells;
    double _seconds;
};

} // namespace Engine

#endif // ATTACKSTRIKEDTO_H
