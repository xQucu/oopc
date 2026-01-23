#include "Blinky.h"
#include <QRandomGenerator>

Blinky::Blinky(const QPointF& pos, int size)
    : Enemy(pos, size, QColor(255, 0, 0))
{
}

void Blinky::updateDirection(const Player&, const QVector<QString>&)
{
    int randomDirection = QRandomGenerator::global()->bounded(4);
    setDesiredDirection(randomDirection);
}
