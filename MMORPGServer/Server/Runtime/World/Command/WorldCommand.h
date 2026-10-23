#ifndef WORLDCOMMAND_H
#define WORLDCOMMAND_H

#include <MMORPGServer/Server/Runtime/World/Command/WorldCommandTypeEnum.h>

namespace Server {

class WorldCommand {
public:
    explicit WorldCommand( WorldCommandTypeEnum type );
    virtual ~WorldCommand();

    WorldCommandTypeEnum type() const;

private:
    WorldCommandTypeEnum _type;
};

} // namespace Server

#endif // WORLDCOMMAND_H
