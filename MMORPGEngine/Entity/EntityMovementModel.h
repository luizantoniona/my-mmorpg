#ifndef ENTITYMOVEMENTMODEL_H
#define ENTITYMOVEMENTMODEL_H

namespace Engine {

class EntityMovementModel {
public:
    EntityMovementModel();

    double cooldownSeconds() const;
    void setCooldownSeconds( double cooldownSeconds );

    int counter() const;
    void setCounter( int counter );

    int cooldownTicks( int tickRate ) const;
    bool isReady( int tickRate ) const;

private:
    double _movementCooldownSeconds;
    int _movementCounter;
};

} // namespace Engine

#endif // ENTITYMOVEMENTMODEL_H
