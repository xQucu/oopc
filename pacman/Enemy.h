#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"
#include "Player.h"
#include <QColor>
#include <QString>
#include <QVector>

int getOppositeDirection(int direction);

class Enemy : public GameObject {
private:
    int direction;
    int desiredDirection;
    QColor color;
    bool frightened;
    QPointF startPosition;
    int moveCounter;

public:
    Enemy(const QPointF& pos, int size, const QColor& c);
    virtual ~Enemy();
    int getDirection() const;
    void setDirection(int d);
    int getDesiredDirection() const;
    void setDesiredDirection(int d);
    void incrementMoveCounter();
    void draw(QPainter& painter) override;
    virtual void updateDirection(const Player& player, const QVector<QString>& map) = 0;
    void update() override;
    bool canMove(int gridX, int gridY, const QVector<QString>& map);
    int countAvailableDirections(const QVector<QString>& map);
    bool isAtIntersection(const QVector<QString>& map);
    void move(const QVector<QString>& map, int);
    void setFrightened(bool f);
    bool isFrightened() const;
    void reset();
    bool collidesWith(const Player& player) const;
};

#endif
