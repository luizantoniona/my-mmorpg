#ifndef ENTITYMOVEMENTMODEL_H
#define ENTITYMOVEMENTMODEL_H

namespace Engine {

class EntityMovementModel {
public:
    EntityMovementModel();

    int cooldown() const;
    void setCooldown( int cooldown );

    int counter() const;
    void setCounter( int counter );

    bool isReady() const;

private:
    int _movementCooldown;
    int _movementCounter;
};

} // namespace Engine

#endif // ENTITYMOVEMENTMODEL_H
