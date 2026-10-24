#ifndef ATTACKCELLMODEL_H
#define ATTACKCELLMODEL_H

namespace Engine {

class AttackCellModel {
public:
    AttackCellModel( int forward, int lateral );

    int forward() const;
    int lateral() const;

private:
    int _forward;
    int _lateral;
};

} // namespace Engine

#endif // ATTACKCELLMODEL_H
