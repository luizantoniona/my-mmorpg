#include "ObserverRegistry.h"

#include <algorithm>

namespace Server {

ObserverRegistry::ObserverRegistry() {
}

void ObserverRegistry::registerCharacterObserver( int idCharacter, const std::shared_ptr<EntityObserver>& observer ) {
    std::lock_guard<std::mutex> lock( _mutex );
    _characterObservers[ idCharacter ] = observer;
}

void ObserverRegistry::unregisterCharacterObserver( int idCharacter ) {
    std::lock_guard<std::mutex> lock( _mutex );
    _characterObservers.erase( idCharacter );
}

void ObserverRegistry::registerGlobalObserver( const std::shared_ptr<EntityObserver>& observer ) {
    std::lock_guard<std::mutex> lock( _mutex );
    _globalObservers.push_back( observer );
}

void ObserverRegistry::unregisterGlobalObserver( const std::shared_ptr<EntityObserver>& observer ) {
    std::lock_guard<std::mutex> lock( _mutex );
    _globalObservers.erase( std::remove( _globalObservers.begin(), _globalObservers.end(), observer ), _globalObservers.end() );
}

std::shared_ptr<EntityObserver> ObserverRegistry::characterObserver( int idCharacter ) {
    std::lock_guard<std::mutex> lock( _mutex );

    auto it = _characterObservers.find( idCharacter );

    return it == _characterObservers.end() ? nullptr : it->second;
}

std::vector<std::shared_ptr<EntityObserver>> ObserverRegistry::globalObservers() {
    std::lock_guard<std::mutex> lock( _mutex );
    return _globalObservers;
}

} // namespace Server
