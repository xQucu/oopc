#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "Enemy.h"
#include "Player.h"
#include <QString>
#include <QTimer>
#include <QVector>
#include <QWidget>

class GameWidget : public QWidget {
    Q_OBJECT

private:
    QTimer* gameTimer;
    Player* player;
    QVector<Enemy*> enemies;
    QVector<QString> map;
    int cellSize;
    int dotsRemaining;
    bool gameOver;
    bool gameWon;
    int globalMoveCounter;

    void initMap();
    void resetGame();
    void checkCollisions();
    void collectDots();
    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent* event) override;

public:
    GameWidget(QWidget* parent = nullptr);
    ~GameWidget();
    void gameLoop();
};

#endif
