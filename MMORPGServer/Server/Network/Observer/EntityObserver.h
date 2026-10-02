#ifndef ENTITYOBSERVER_H
#define ENTITYOBSERVER_H

#include <string>

namespace Server {

class EntityObserver {
public:
    virtual ~EntityObserver();

    virtual void send( const std::string& message ) = 0;

    virtual void shutdown() = 0;
};

} // namespace Server

#endif // ENTITYOBSERVER_H
