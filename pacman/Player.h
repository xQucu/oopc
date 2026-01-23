#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include <QColor>
#include <QString>
#include <QVector>

class Player : public GameObject {
private:
    int direction;
    int nextDirection;
    bool powered;
    int powerTimer;
    int score;
    int lives;
    int mouthAngle;

public:
    Player(const QPointF& pos, int size);
    void draw(QPainter& painter) override;
    void update() override;
    void handleKeyPress(int key);
    bool canMove(int gridX, int gridY, const QVector<QString>& map);
    void move(const QVector<QString>& map, int);
    void setPowered(bool p);
    bool isPowered() const;
    void addScore(int points);
    int getScore() const;
    int getLives() const;
    void loseLife();
    int getDirection() const;
    void resetPosition();
};

#endif
