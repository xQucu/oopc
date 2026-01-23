#include "GameWidget.h"
#include "Blinky.h"
#include <QApplication>
#include <QKeyEvent>
#include <QPainter>

GameWidget::GameWidget(QWidget* parent)
    : QWidget(parent)
    , cellSize(30)
    , dotsRemaining(0)
    , gameOver(false)
    , gameWon(false)
    , globalMoveCounter(0)
{
    setFocusPolicy(Qt::StrongFocus);
    initMap();

    player = new Player(QPointF(cellSize, cellSize), cellSize);

    enemies.push_back(new Blinky(QPointF(13 * cellSize, 11 * cellSize), cellSize));

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameWidget::gameLoop);
    gameTimer->start(16);

    setMinimumSize(map[0].length() * cellSize, map.size() * cellSize);
}

GameWidget::~GameWidget()
{
    delete player;
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
}

void GameWidget::initMap()
{
    map = {
        "############################",
        "#............##............#",
        "#.####.#####.##.#####.####.#",
        "#O#  #.#   #.##.#   #.#  #O#",
        "#.####.#####.##.#####.####.#",
        "#..........................#",
        "#.####.##.########.##.####.#",
        "#.####.##.########.##.####.#",
        "#......##....##....##......#",
        "######.#####.##.#####.######",
        "######.#####.##.#####.######",
        "######.##          ##.######",
        "######.## ###  ### ##.######",
        "######.## #      # ##.######",
        "######... #      # ...######",
        "######.## #      # ##.######",
        "######.## ######## ##.######",
        "######.##          ##.######",
        "######.## ######## ##.######",
        "######.## ######## ##.######",
        "#............##............#",
        "#.####.#####.##.#####.####.#",
        "#.####.#####.##.#####.####.#",
        "#O..##.......  .......##..O#",
        "###.##.##.########.##.##.###",
        "###.##.##.########.##.##.###",
        "#......##....##....##......#",
        "#.##########.##.##########.#",
        "#.##########.##.##########.#",
        "#..........................#",
        "############################"
    };

    dotsRemaining = 0;
    for (const QString& row : map) {
        for (QChar c : row) {
            if (c == '.' || c == 'O') {
                dotsRemaining++;
            }
        }
    }
}

void GameWidget::resetGame()
{
    gameOver = false;
    gameWon = false;

    initMap();
    player->resetPosition();

    for (Enemy* enemy : enemies) {
        enemy->reset();
    }
}

void GameWidget::checkCollisions()
{
    for (Enemy* enemy : enemies) {
        if (enemy->collidesWith(*player)) {
            if (player->isPowered()) {
                player->addScore(200);
                enemy->reset();
            } else {
                player->loseLife();
                if (player->getLives() <= 0) {
                    gameOver = true;
                } else {
                    player->resetPosition();
                    for (Enemy* e : enemies) {
                        e->reset();
                    }
                }
            }
        }
    }

    if (!player->isPowered()) {
        for (Enemy* enemy : enemies) {
            enemy->setFrightened(false);
        }
    }
}

void GameWidget::collectDots()
{
    int x = player->getGridX();
    int y = player->getGridY();

    if (y >= 0 && y < map.size() && x >= 0 && x < map[y].length()) {
        QChar cell = map[y][x];

        if (cell == '.') {
            map[y][x] = ' ';
            player->addScore(10);
            dotsRemaining--;
        } else if (cell == 'O') {
            map[y][x] = ' ';
            player->addScore(50);
            player->setPowered(true);
            dotsRemaining--;

            for (Enemy* enemy : enemies) {
                enemy->setFrightened(true);
            }
        }
    }
}

void GameWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), Qt::black);

    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].length(); x++) {
            QChar cell = map[y][x];

            if (cell == '#') {
                painter.setPen(QPen(QColor(33, 33, 255), 3));

                if (y > 0 && map[y - 1][x] != '#') {
                    painter.drawLine(x * cellSize, y * cellSize,
                        (x + 1) * cellSize, y * cellSize);
                }
                if (y < map.size() - 1 && map[y + 1][x] != '#') {
                    painter.drawLine(x * cellSize, (y + 1) * cellSize,
                        (x + 1) * cellSize, (y + 1) * cellSize);
                }
                if (x > 0 && map[y][x - 1] != '#') {
                    painter.drawLine(x * cellSize, y * cellSize,
                        x * cellSize, (y + 1) * cellSize);
                }
                if (x < map[y].length() - 1 && map[y][x + 1] != '#') {
                    painter.drawLine((x + 1) * cellSize, y * cellSize,
                        (x + 1) * cellSize, (y + 1) * cellSize);
                }
            } else if (cell == '.') {
                painter.setBrush(Qt::white);
                painter.setPen(Qt::NoPen);
                painter.drawEllipse(x * cellSize + cellSize / 2 - 2,
                    y * cellSize + cellSize / 2 - 2, 4, 4);
            } else if (cell == 'O') {
                painter.setBrush(Qt::white);
                painter.setPen(Qt::NoPen);
                painter.drawEllipse(x * cellSize + cellSize / 2 - 6,
                    y * cellSize + cellSize / 2 - 6, 12, 12);
            }
        }
    }

    player->draw(painter);
    for (Enemy* enemy : enemies) {
        enemy->draw(painter);
    }

    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(14);
    painter.setFont(font);
    painter.drawText(10, height() - 10,
        QString("Score: %1  Lives: %2").arg(player->getScore()).arg(player->getLives()));

    if (gameOver) {
        font.setPointSize(32);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter, "GAME OVER");
    } else if (gameWon) {
        font.setPointSize(32);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter, "YOU WIN!");
    }
}

void GameWidget::keyPressEvent(QKeyEvent* event)
{
    if (gameOver || gameWon) {
        QApplication::quit();
        return;
    }

    player->handleKeyPress(event->key());
}

void GameWidget::gameLoop()
{
    if (gameOver || gameWon)
        return;

    globalMoveCounter++;

    player->update();
    player->move(map, globalMoveCounter);

    collectDots();

    for (Enemy* enemy : enemies) {
        enemy->update();
        enemy->updateDirection(*player, map);
        enemy->move(map, globalMoveCounter);
    }

    checkCollisions();

    if (dotsRemaining == 0) {
        gameWon = true;
    }

    update();
}
