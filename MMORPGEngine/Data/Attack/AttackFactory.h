#ifndef ATTACKFACTORY_H
#define ATTACKFACTORY_H

#include <json/json.h>

#include <MMORPGEngine/Data/Attack/AttackModel.h>

namespace Engine {

class AttackFactory {
public:
    static bool fromJson( const Json::Value& json, AttackModel& attack );
};

} // namespace Engine

#endif // ATTACKFACTORY_H
