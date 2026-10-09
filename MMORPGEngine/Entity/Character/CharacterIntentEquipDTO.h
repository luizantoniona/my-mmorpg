#ifndef CHARACTERINTENTEQUIPDTO_H
#define CHARACTERINTENTEQUIPDTO_H

#include <cstdint>
#include <optional>

#include <json/json.h>

namespace Engine {

class CharacterIntentEquipDTO {
public:
    CharacterIntentEquipDTO();
    ~CharacterIntentEquipDTO();

    static CharacterIntentEquipDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    const std::optional<uint32_t>& position() const;
    void setPosition( uint32_t position );

private:
    std::optional<uint32_t> _position;
};

} // namespace Engine

#endif // CHARACTERINTENTEQUIPDTO_H
