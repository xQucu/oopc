#ifndef BLINKY_H
#define BLINKY_H

#include "Enemy.h"

class Blinky : public Enemy {
public:
    Blinky(const QPointF& pos, int size);
    void updateDirection(const Player&, const QVector<QString>&) override;
};

#endif
