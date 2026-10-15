#ifndef CHARACTERINTENTDROPDTO_H
#define CHARACTERINTENTDROPDTO_H

#include <cstdint>
#include <optional>

#include <json/json.h>

namespace Engine {

class CharacterIntentDropDTO {
public:
    CharacterIntentDropDTO();
    ~CharacterIntentDropDTO();

    static CharacterIntentDropDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    const std::optional<uint32_t>& position() const;
    void setPosition( uint32_t position );

private:
    std::optional<uint32_t> _position;
};

} // namespace Engine

#endif // CHARACTERINTENTDROPDTO_H
