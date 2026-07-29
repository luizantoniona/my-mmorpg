
#include <MMORPGEngine/Core/World/Model/WorldModel.h>

class WorldPageControl : public QObject {
    Q_OBJECT
public:
    explicit WorldPageControl( QObject* parent = nullptr );

private:
    std::unique_ptr<Engine::WorldModel> _world;
};

#endif // WORLDPAGECONTROL_H
