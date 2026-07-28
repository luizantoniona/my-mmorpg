#ifndef GROUNDMODEL_H
#define GROUNDMODEL_H

#include <cstdint>
#include <string>

namespace Engine {

class GroundModel {
public:
    GroundModel();

    uint16_t type() const;
    void setType( uint16_t type );

    bool walkable() const;
    void setWalkable( bool walkable );

    std::string name() const;
    void setName( const std::string& name );

    std::string folder() const;
    void setFolder( const std::string& folder );

private:
    uint16_t _type;
    bool _walkable;
    std::string _name;
    std::string _folder;
};

} // namespace Engine

#endif // GROUNDMODEL_H
