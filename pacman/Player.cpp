#include "Player.h"
#include <Qt>

Player::Player(const QPointF& pos, int size)
    : GameObject(pos, size)
    , direction(0)
    , nextDirection(0)
    , powered(false)
    , powerTimer(0)
    , score(0)
    , lives(3)
    , mouthAngle(0)
{
    speed = 2.5f;
}

void Player::draw(QPainter& painter)
{
    painter.save();

    if (powered) {
        painter.setBrush(QColor(255, 255, 100));
    } else {
        painter.setBrush(Qt::yellow);
    }

    painter.setPen(Qt::NoPen);

    int margin = cellSize / 6;

    int mouthDegrees = (mouthAngle % 10) * 2;

    int startAngle = 0;
    switch (direction) {
    case 0:
        startAngle = 0 + mouthDegrees / 2;
        break;
    case 1:
        startAngle = 270 + mouthDegrees / 2;
        break;
    case 2:
        startAngle = 180 + mouthDegrees / 2;
        break;
    case 3:
        startAngle = 90 + mouthDegrees / 2;
        break;
    }

    painter.drawPie(QRectF(position.x() + margin, position.y() + margin,
                        cellSize - 2 * margin, cellSize - 2 * margin),
        startAngle * 16, (360 - mouthDegrees) * 16);

    painter.restore();
}

void Player::update()
{
    if (powered && powerTimer > 0) {
        powerTimer--;
        if (powerTimer == 0) {
            powered = false;
        }
    }

    mouthAngle = (mouthAngle + 2) % 15;

    moveTowardsTarget();
}

void Player::handleKeyPress(int key)
{
    switch (key) {
    case Qt::Key_Left:
        nextDirection = 2;
        break;
    case Qt::Key_Right:
        nextDirection = 0;
        break;
    case Qt::Key_Up:
        nextDirection = 3;
        break;
    case Qt::Key_Down:
        nextDirection = 1;
        break;
    }
}

bool Player::canMove(int gridX, int gridY, const QVector<QString>& map)
{
    if (gridX < 0 || gridX >= map[0].length()) {
        return true;
    }
    if (gridY < 0 || gridY >= map.size())
        return false;
    QChar cell = map[gridY][gridX];
    return cell != '#';
}

void Player::move(const QVector<QString>& map, int)
{
    if (!isAtTarget())
        return;

    int currentX = getGridX();
    int currentY = getGridY();

    int testX = currentX;
    int testY = currentY;

    switch (nextDirection) {
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
        direction = nextDirection;
    } else {
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

        if (!canMove(testX, testY, map)) {
            return;
        }
    }

    int nextX = currentX;
    int nextY = currentY;

    switch (direction) {
    case 0:
        nextX++;
        break;
    case 1:
        nextY++;
        break;
    case 2:
        nextX--;
        break;
    case 3:
        nextY--;
        break;
    }

    if (canMove(nextX, nextY, map)) {
        if (nextX < 0) {
            targetPosition.setX((map[0].length() - 1) * cellSize);
            targetPosition.setY(currentY * cellSize);
            position.setX(-cellSize);
            position.setY(currentY * cellSize);
        } else if (nextX >= map[0].length()) {
            targetPosition.setX(0);
            targetPosition.setY(currentY * cellSize);
            position.setX(map[0].length() * cellSize);
            position.setY(currentY * cellSize);
        } else {
            targetPosition.setX(nextX * cellSize);
            targetPosition.setY(nextY * cellSize);
        }
    }
}

void Player::setPowered(bool p)
{
    powered = p;
    powerTimer = powered ? 200 : 0;
}

bool Player::isPowered() const { return powered; }

void Player::addScore(int points) { score += points; }

int Player::getScore() const { return score; }

int Player::getLives() const { return lives; }

void Player::loseLife() { lives--; }

int Player::getDirection() const { return direction; }

void Player::resetPosition()
{
    position = QPointF(cellSize, cellSize);
    targetPosition = position;
    direction = 0;
    nextDirection = 0;
    mouthAngle = 0;
}
