#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QPainter>
#include <QPointF>
#include <QtMath>

class GameObject {
protected:
    QPointF position;
    QPointF targetPosition;
    int cellSize;
    float speed;
    bool isMoving;

public:
    GameObject(const QPointF& pos, int size);
    virtual ~GameObject();
    virtual void draw(QPainter& painter) = 0;
    virtual void update();
    QPointF getPosition() const;
    void setPosition(const QPointF& pos);
    int getGridX() const;
    int getGridY() const;
    bool isAtTarget() const;
    void moveTowardsTarget();
};

#endif
