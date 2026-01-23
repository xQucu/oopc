#include "GameObject.h"

GameObject::GameObject(const QPointF& pos, int size)
    : position(pos)
    , targetPosition(pos)
    , cellSize(size)
    , speed(2.0f)
    , isMoving(false)
{
}

GameObject::~GameObject() { }

void GameObject::update() { }

QPointF GameObject::getPosition() const { return position; }

void GameObject::setPosition(const QPointF& pos)
{
    position = pos;
    targetPosition = pos;
}

int GameObject::getGridX() const { return qRound(position.x() / cellSize); }

int GameObject::getGridY() const { return qRound(position.y() / cellSize); }

bool GameObject::isAtTarget() const
{
    float dx = targetPosition.x() - position.x();
    float dy = targetPosition.y() - position.y();
    return (dx * dx + dy * dy) < 1.0f;
}

void GameObject::moveTowardsTarget()
{
    if (isAtTarget()) {
        position = targetPosition;
        isMoving = false;
        return;
    }

    float dx = targetPosition.x() - position.x();
    float dy = targetPosition.y() - position.y();
    float distance = sqrt(dx * dx + dy * dy);

    if (distance <= speed) {
        position = targetPosition;
        isMoving = false;
    } else {
        position.setX(position.x() + (dx / distance) * speed);
        position.setY(position.y() + (dy / distance) * speed);
        isMoving = true;
    }
}
