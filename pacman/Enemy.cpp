#include "Enemy.h"
#include <QRandomGenerator>
#include <QtMath>

int selectPreferredDirection(int dx, int dy)
{
    if (abs(dx) > abs(dy)) {
        return (dx > 0) ? 0 : 2;
    } else if (abs(dy) > abs(dx)) {
        return (dy > 0) ? 1 : 3;
    } else {
        if (dx > 0)
            return 0;
        if (dy > 0)
            return 1;
        if (dx < 0)
            return 2;
        return 3;
    }
}

int getOppositeDirection(int direction)
{
    return (direction + 2) % 4;
}

Enemy::Enemy(const QPointF& pos, int size, const QColor& c)
    : GameObject(pos, size)
    , direction(0)
    , desiredDirection(0)
    , color(c)
    , frightened(false)
    , startPosition(pos)
    , moveCounter(0)
{
    speed = 2.0f;
}

Enemy::~Enemy() { }

int Enemy::getDirection() const { return direction; }

void Enemy::setDirection(int d) { direction = d; }

int Enemy::getDesiredDirection() const { return desiredDirection; }

void Enemy::setDesiredDirection(int d) { desiredDirection = d; }

int Enemy::getMoveCounter() const { return moveCounter; }

void Enemy::incrementMoveCounter() { moveCounter++; }

void Enemy::resetMoveCounter() { moveCounter = 0; }

int Enemy::getGhostGridX() const { return qRound(position.x() / cellSize); }

int Enemy::getGhostGridY() const { return qRound(position.y() / cellSize); }

void Enemy::draw(QPainter& painter)
{
    painter.save();

    if (frightened) {
        painter.setBrush(QColor(50, 50, 200));
    } else {
        painter.setBrush(color);
    }

    painter.setPen(Qt::NoPen);

    int margin = cellSize / 6;
    painter.drawEllipse(position.x() + margin, position.y() + margin,
        cellSize - 2 * margin, cellSize - 2 * margin);

    painter.setBrush(Qt::white);
    int eyeSize = cellSize / 5;
    painter.drawEllipse(position.x() + cellSize / 3.0, position.y() + cellSize / 3.0,
        eyeSize, eyeSize);
    painter.drawEllipse(position.x() + 2.0 * cellSize / 3.0 - eyeSize, position.y() + cellSize / 3.0,
        eyeSize, eyeSize);

    painter.restore();
}

void Enemy::update()
{
    moveTowardsTarget();
}

bool Enemy::canMove(int gridX, int gridY, const QVector<QString>& map)
{
    if (gridX < 0 || gridX >= map[0].length()) {
        return true;
    }
    if (gridY < 0 || gridY >= map.size())
        return false;
    QChar cell = map[gridY][gridX];
    return cell != '#';
}

int Enemy::countAvailableDirections(const QVector<QString>& map)
{
    int currentX = getGridX();
    int currentY = getGridY();
    int count = 0;

    if (canMove(currentX + 1, currentY, map))
        count++;
    if (canMove(currentX, currentY + 1, map))
        count++;
    if (canMove(currentX - 1, currentY, map))
        count++;
    if (canMove(currentX, currentY - 1, map))
        count++;

    return count;
}

bool Enemy::isAtIntersection(const QVector<QString>& map)
{
    return countAvailableDirections(map) >= 3;
}

void Enemy::move(const QVector<QString>& map, int)
{
    if (!isAtTarget())
        return;

    incrementMoveCounter();

    int currentX = getGridX();
    int currentY = getGridY();
    int newX = currentX;
    int newY = currentY;

    bool atIntersection = isAtIntersection(map);

    int directionToUse = getDirection();

    if (atIntersection) {
        int desiredDir = getDesiredDirection();
        int oppositeDir = getOppositeDirection(getDirection());
        int testX = currentX;
        int testY = currentY;

        switch (desiredDir) {
        case 0:
            testX++;
            break;
        case 1:
            testY++;
            break;
        case 2:
            testX--;
            break;
        case 3:
            testY--;
            break;
        }

        if (canMove(testX, testY, map) && desiredDir != oppositeDir) {
            directionToUse = desiredDir;
            setDirection(directionToUse);
        } else {
            testX = currentX;
            testY = currentY;

            switch (getDirection()) {
            case 0:
                testX++;
                break;
            case 1:
                testY++;
                break;
            case 2:
                testX--;
                break;
            case 3:
                testY--;
                break;
            }

            if (canMove(testX, testY, map)) {
                directionToUse = getDirection();
            } else {
                bool found = false;
                int preferredOrder[] = { 3, 0, 1, 2 };
                int oppositeDir = getOppositeDirection(getDirection());

                for (int i = 0; i < 4; i++) {
                    int direction = preferredOrder[i];

                    if (direction == oppositeDir)
                        continue;

                    testX = currentX;
                    testY = currentY;

                    switch (direction) {
                    case 0:
                        testX++;
                        break;
                    case 1:
                        testY++;
                        break;
                    case 2:
                        testX--;
                        break;
                    case 3:
                        testY--;
                        break;
                    }

                    if (canMove(testX, testY, map)) {
                        directionToUse = direction;
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    for (int attempt = 0; attempt < 10; attempt++) {
                        int randomDir = QRandomGenerator::global()->bounded(4);

                        if (randomDir == oppositeDir)
                            continue;

                        testX = currentX;
                        testY = currentY;

                        switch (randomDir) {
                        case 0:
                            testX++;
                            break;
                        case 1:
                            testY++;
                            break;
                        case 2:
                            testX--;
                            break;
                        case 3:
                            testY--;
                            break;
                        }

                        if (canMove(testX, testY, map)) {
                            directionToUse = randomDir;
                            break;
                        }
                    }
                }
                setDirection(directionToUse);
            }
        }
    }

    newX = currentX;
    newY = currentY;

    switch (directionToUse) {
    case 0:
        newX++;
        break;
    case 1:
        newY++;
        break;
    case 2:
        newX--;
        break;
    case 3:
        newY--;
        break;
    }

    if (canMove(newX, newY, map)) {
        if (newX < 0) {
            targetPosition.setX((map[0].length() - 1) * cellSize);
            targetPosition.setY(currentY * cellSize);
            position.setX(-cellSize);
            position.setY(currentY * cellSize);
        } else if (newX >= map[0].length()) {
            targetPosition.setX(0);
            targetPosition.setY(currentY * cellSize);
            position.setX(map[0].length() * cellSize);
            position.setY(currentY * cellSize);
        } else {
            targetPosition.setX(newX * cellSize);
            targetPosition.setY(newY * cellSize);
        }
    } else {
        bool found = false;
        int preferredOrder[] = { 3, 0, 1, 2 };
        int oppositeDir = getOppositeDirection(getDirection());

        for (int i = 0; i < 4; i++) {
            int direction = preferredOrder[i];

            if (direction == oppositeDir)
                continue;

            newX = currentX;
            newY = currentY;

            switch (direction) {
            case 0:
                newX++;
                break;
            case 1:
                newY++;
                break;
            case 2:
                newX--;
                break;
            case 3:
                newY--;
                break;
            }

            if (canMove(newX, newY, map)) {
                setDirection(direction);
                targetPosition.setX(newX * cellSize);
                targetPosition.setY(newY * cellSize);
                found = true;
                break;
            }
        }

        if (!found) {
            for (int attempt = 0; attempt < 10; attempt++) {
                int randomDir = QRandomGenerator::global()->bounded(4);

                if (randomDir == oppositeDir)
                    continue;

                newX = currentX;
                newY = currentY;

                switch (randomDir) {
                case 0:
                    newX++;
                    break;
                case 1:
                    newY++;
                    break;
                case 2:
                    newX--;
                    break;
                case 3:
                    newY--;
                    break;
                }

                if (canMove(newX, newY, map)) {
                    setDirection(randomDir);
                    targetPosition.setX(newX * cellSize);
                    targetPosition.setY(newY * cellSize);
                    break;
                }
            }
        }
    }
}

void Enemy::setFrightened(bool f) { frightened = f; }

bool Enemy::isFrightened() const { return frightened; }

void Enemy::reset()
{
    position = startPosition;
    targetPosition = startPosition;
    setDirection(QRandomGenerator::global()->bounded(4));
    setFrightened(false);
    resetMoveCounter();
}

bool Enemy::collidesWith(const Player& player) const
{
    float dx = position.x() - player.getPosition().x();
    float dy = position.y() - player.getPosition().y();
    float distance = sqrt(dx * dx + dy * dy);
    return distance < cellSize * 0.7f;
}
