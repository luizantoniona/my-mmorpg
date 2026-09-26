#ifndef WORLDCOMMAND_H
#define WORLDCOMMAND_H

namespace Server {

class WorldRuntime;

class WorldCommand {
public:
    virtual ~WorldCommand();

    virtual void execute( WorldRuntime& runtime ) = 0;
};

} // namespace Server

#endif // WORLDCOMMAND_H
