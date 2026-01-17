#include <QApplication>
#include <QColor>
#include <QKeyEvent>
#include <QPainter>
#include <QPointF>
#include <QRandomGenerator>
#include <QString>
#include <QTimer>
#include <QVector>
#include <QWidget>
#include <QtMath>

// ============================================================================
// GameObject - Base class
// ============================================================================
class GameObject {
protected:
    QPointF position;
    QPointF targetPosition;
    int cellSize;
    float speed;
    bool isMoving;

public:
    GameObject(const QPointF& pos, int size)
        : position(pos)
        , targetPosition(pos)
        , cellSize(size)
        , speed(2.0f)
        , isMoving(false)
    {
    }

    virtual ~GameObject() { }

    virtual void draw(QPainter& painter) = 0;

    virtual void update() { }

    QPointF getPosition() const { return position; }

    void setPosition(const QPointF& pos)
    {
        position = pos;
        targetPosition = pos;
    }

    int getGridX() const { return qRound(position.x() / cellSize); }

    int getGridY() const { return qRound(position.y() / cellSize); }

    int getTargetGridX() const { return qRound(targetPosition.x() / cellSize); }

    int getTargetGridY() const { return qRound(targetPosition.y() / cellSize); }

    bool isAtTarget() const
    {
        float dx = targetPosition.x() - position.x();
        float dy = targetPosition.y() - position.y();
        return (dx * dx + dy * dy) < 1.0f;
    }

    void moveTowardsTarget()
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
};

// ============================================================================
// Player class
// ============================================================================
class Player : public GameObject {
private:
    int direction; // 0=right, 1=down, 2=left, 3=up
    int nextDirection;
    bool powered;
    int powerTimer;
    int score;
    int lives;

public:
    Player(const QPointF& pos, int size)
        : GameObject(pos, size)
        , direction(0)
        , nextDirection(0)
        , powered(false)
        , powerTimer(0)
        , score(0)
        , lives(3)
    {
        speed = 2.5f;
    }

    void draw(QPainter& painter) override
    {
        painter.save();

        if (powered) {
            painter.setBrush(QColor(255, 255, 100));
        } else {
            painter.setBrush(Qt::yellow);
        }

        painter.setPen(Qt::NoPen);

        int margin = cellSize / 6;
        painter.drawEllipse(position.x() + margin, position.y() + margin,
            cellSize - 2 * margin, cellSize - 2 * margin);

        painter.restore();
    }

    void update() override
    {
        if (powered && powerTimer > 0) {
            powerTimer--;
            if (powerTimer == 0) {
                powered = false;
            }
        }

        moveTowardsTarget();
    }

    void handleKeyPress(int key)
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

    bool canMove(int gridX, int gridY, const QVector<QString>& map)
    {
        // Allow movement outside bounds for tunnel
        if (gridX < 0 || gridX >= map[0].length()) {
            return true;
        }
        if (gridY < 0 || gridY >= map.size())
            return false;
        QChar cell = map[gridY][gridX];
        return cell != '#' && cell != '=';
    }

    void move(const QVector<QString>& map, int)
    {
        // Only set new target when reached current target
        if (!isAtTarget())
            return;

        int currentX = getGridX();
        int currentY = getGridY();
        int nextX = currentX;
        int nextY = currentY;

        // Try to change direction
        switch (nextDirection) {
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
            direction = nextDirection;
        } else {
            // Continue in current direction
            nextX = currentX;
            nextY = currentY;
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
        }

        if (canMove(nextX, nextY, map)) {
            // Handle tunnel wrapping
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

    void setPowered(bool p)
    {
        powered = p;
        powerTimer = powered ? 200 : 0;
    }

    bool isPowered() const { return powered; }

    void addScore(int points) { score += points; }

    int getScore() const { return score; }

    int getLives() const { return lives; }

    void loseLife() { lives--; }

    int getDirection() const { return direction; }

    void resetPosition()
    {
        position = QPointF(cellSize, cellSize);
        targetPosition = position;
        direction = 0;
        nextDirection = 0;
    }
};

// ============================================================================
// Enemy base class
// ============================================================================
class Enemy : public GameObject {
protected:
    int direction;
    QColor color;
    bool frightened;
    QPointF startPosition;

public:
    Enemy(const QPointF& pos, int size, const QColor& c)
        : GameObject(pos, size)
        , direction(0)
        , color(c)
        , frightened(false)
        , startPosition(pos)
    {
        speed = 2.0f;
    }

    virtual ~Enemy() { }

    void draw(QPainter& painter) override
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

        // Draw eyes
        painter.setBrush(Qt::white);
        int eyeSize = cellSize / 5;
        painter.drawEllipse(position.x() + cellSize / 3, position.y() + cellSize / 3,
            eyeSize, eyeSize);
        painter.drawEllipse(position.x() + 2 * cellSize / 3 - eyeSize, position.y() + cellSize / 3,
            eyeSize, eyeSize);

        painter.restore();
    }

    virtual void updateDirection(const Player& player, const QVector<QString>& map) = 0;

    void update() override
    {
        moveTowardsTarget();
    }

    bool canMove(int gridX, int gridY, const QVector<QString>& map)
    {
        // Allow movement outside bounds for tunnel
        if (gridX < 0 || gridX >= map[0].length()) {
            return true;
        }
        if (gridY < 0 || gridY >= map.size())
            return false;
        QChar cell = map[gridY][gridX];
        return cell != '#' && cell != '=';
    }

    void move(const QVector<QString>& map, int)
    {
        // Only set new target when reached current target
        if (!isAtTarget())
            return;

        int currentX = getGridX();
        int currentY = getGridY();
        int newX = currentX;
        int newY = currentY;

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
            // Handle tunnel wrapping
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
            direction = QRandomGenerator::global()->bounded(4);
        }
    }

    void setFrightened(bool f) { frightened = f; }

    bool isFrightened() const { return frightened; }

    void reset()
    {
        position = startPosition;
        targetPosition = startPosition;
        direction = 0;
        frightened = false;
    }

    bool collidesWith(const Player& player) const
    {
        float dx = position.x() - player.getPosition().x();
        float dy = position.y() - player.getPosition().y();
        float distance = sqrt(dx * dx + dy * dy);
        return distance < cellSize * 0.7f;
    }
};

// ============================================================================
// Blinky - chases player directly
// ============================================================================
class Blinky : public Enemy {
public:
    Blinky(const QPointF& pos, int size)
        : Enemy(pos, size, QColor(255, 0, 0))
    {
    }

    void updateDirection(const Player& player, const QVector<QString>&) override
    {
        if (frightened) {
            // Run away from player
            int dx = getGridX() - player.getGridX();
            int dy = getGridY() - player.getGridY();

            if (abs(dx) > abs(dy)) {
                direction = (dx > 0) ? 0 : 2;
            } else {
                direction = (dy > 0) ? 1 : 3;
            }
        } else {
            // Chase player directly
            int dx = player.getGridX() - getGridX();
            int dy = player.getGridY() - getGridY();

            if (abs(dx) > abs(dy)) {
                direction = (dx > 0) ? 0 : 2;
            } else {
                direction = (dy > 0) ? 1 : 3;
            }
        }
    }
};

// ============================================================================
// Pinky - tries to ambush player ahead
// ============================================================================
class Pinky : public Enemy {
public:
    Pinky(const QPointF& pos, int size)
        : Enemy(pos, size, QColor(255, 184, 255))
    {
    }

    void updateDirection(const Player& player, const QVector<QString>&) override
    {
        if (frightened) {
            direction = QRandomGenerator::global()->bounded(4);
        } else {
            // Target 4 cells ahead of player
            int targetX = player.getGridX();
            int targetY = player.getGridY();

            switch (player.getDirection()) {
            case 0:
                targetX += 4;
                break;
            case 1:
                targetY += 4;
                break;
            case 2:
                targetX -= 4;
                break;
            case 3:
                targetY -= 4;
                break;
            }

            int dx = targetX - getGridX();
            int dy = targetY - getGridY();

            if (abs(dx) > abs(dy)) {
                direction = (dx > 0) ? 0 : 2;
            } else {
                direction = (dy > 0) ? 1 : 3;
            }
        }
    }
};

// ============================================================================
// Inky - uses complex pattern based on Blinky and player
// ============================================================================
class Inky : public Enemy {
private:
    const Blinky* blinky;

public:
    Inky(const QPointF& pos, int size, const Blinky* b)
        : Enemy(pos, size, QColor(0, 255, 255))
        , blinky(b)
    {
    }

    void updateDirection(const Player& player, const QVector<QString>&) override
    {
        if (frightened) {
            direction = QRandomGenerator::global()->bounded(4);
        } else {
            // Complex targeting based on Blinky and player
            int playerX = player.getGridX();
            int playerY = player.getGridY();

            switch (player.getDirection()) {
            case 0:
                playerX += 2;
                break;
            case 1:
                playerY += 2;
                break;
            case 2:
                playerX -= 2;
                break;
            case 3:
                playerY -= 2;
                break;
            }

            int targetX = playerX + (playerX - blinky->getGridX());
            int targetY = playerY + (playerY - blinky->getGridY());

            int dx = targetX - getGridX();
            int dy = targetY - getGridY();

            if (abs(dx) > abs(dy)) {
                direction = (dx > 0) ? 0 : 2;
            } else {
                direction = (dy > 0) ? 1 : 3;
            }
        }
    }
};

// ============================================================================
// Clyde - chases when far, scatters when close
// ============================================================================
class Clyde : public Enemy {
public:
    Clyde(const QPointF& pos, int size)
        : Enemy(pos, size, QColor(255, 184, 82))
    {
    }

    void updateDirection(const Player& player, const QVector<QString>& map) override
    {
        int dx = player.getGridX() - getGridX();
        int dy = player.getGridY() - getGridY();
        int distance = dx * dx + dy * dy;

        if (frightened || distance < 64) {
            // Scatter to corner when close or frightened
            int targetX = 0;
            int targetY = map.size() - 1;

            dx = targetX - getGridX();
            dy = targetY - getGridY();
        }

        if (abs(dx) > abs(dy)) {
            direction = (dx > 0) ? 0 : 2;
        } else {
            direction = (dy > 0) ? 1 : 3;
        }
    }
};

// ============================================================================
// GameWidget - Main game widget
// ============================================================================
class GameWidget : public QWidget {
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

    void initMap()
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
            "######.## ###==### ##.######",
            "######.## #      # ##.######",
            "     ...  #      # ...      ",
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

        // Count dots
        dotsRemaining = 0;
        for (const QString& row : map) {
            for (QChar c : row) {
                if (c == '.' || c == 'O') {
                    dotsRemaining++;
                }
            }
        }
    }

    void resetGame()
    {
        gameOver = false;
        gameWon = false;

        initMap();
        player->resetPosition();

        for (Enemy* enemy : enemies) {
            enemy->reset();
        }
    }

    void checkCollisions()
    {
        for (Enemy* enemy : enemies) {
            if (enemy->collidesWith(*player)) {
                if (player->isPowered()) {
                    // Eat ghost
                    player->addScore(200);
                    enemy->reset();
                } else {
                    // Lose life
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

    void collectDots()
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

    void paintEvent(QPaintEvent*) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Draw background
        painter.fillRect(rect(), Qt::black);

        // Draw map - only wall borders
        for (int y = 0; y < map.size(); y++) {
            for (int x = 0; x < map[y].length(); x++) {
                QChar cell = map[y][x];

                if (cell == '#') {
                    // Check neighbors to draw borders only where walls meet paths
                    painter.setPen(QPen(QColor(33, 33, 255), 3));

                    // Check top
                    if (y > 0 && map[y - 1][x] != '#') {
                        painter.drawLine(x * cellSize, y * cellSize,
                            (x + 1) * cellSize, y * cellSize);
                    }
                    // Check bottom
                    if (y < map.size() - 1 && map[y + 1][x] != '#') {
                        painter.drawLine(x * cellSize, (y + 1) * cellSize,
                            (x + 1) * cellSize, (y + 1) * cellSize);
                    }
                    // Check left
                    if (x > 0 && map[y][x - 1] != '#') {
                        painter.drawLine(x * cellSize, y * cellSize,
                            x * cellSize, (y + 1) * cellSize);
                    }
                    // Check right
                    if (x < map[y].length() - 1 && map[y][x + 1] != '#') {
                        painter.drawLine((x + 1) * cellSize, y * cellSize,
                            (x + 1) * cellSize, (y + 1) * cellSize);
                    }
                } else if (cell == '=') {
                    // Draw ghost house gate
                    painter.setPen(QPen(QColor(255, 184, 255), 3));
                    painter.drawLine(x * cellSize, y * cellSize + cellSize / 2,
                        (x + 1) * cellSize, y * cellSize + cellSize / 2);
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

        // Draw game objects
        player->draw(painter);
        for (Enemy* enemy : enemies) {
            enemy->draw(painter);
        }

        // Draw HUD
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

    void keyPressEvent(QKeyEvent* event) override
    {
        if (gameOver || gameWon) {
            if (event->key() == Qt::Key_Space) {
                resetGame();
            }
            return;
        }

        player->handleKeyPress(event->key());
    }

public:
    void gameLoop()
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

    GameWidget(QWidget* parent = nullptr)
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

        Blinky* blinky = new Blinky(QPointF(13 * cellSize, 11 * cellSize), cellSize);
        enemies.push_back(blinky);
        enemies.push_back(new Pinky(QPointF(11 * cellSize, 11 * cellSize), cellSize));
        enemies.push_back(new Inky(QPointF(13 * cellSize, 13 * cellSize), cellSize, blinky));
        enemies.push_back(new Clyde(QPointF(15 * cellSize, 11 * cellSize), cellSize));

        gameTimer = new QTimer(this);
        connect(gameTimer, &QTimer::timeout, this, &GameWidget::gameLoop);
        gameTimer->start(16); // 16ms = ~60 FPS

        setMinimumSize(map[0].length() * cellSize, map.size() * cellSize);
    }

    ~GameWidget()
    {
        delete player;
        for (Enemy* enemy : enemies) {
            delete enemy;
        }
    }
};

// ============================================================================
// Main function
// ============================================================================
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    GameWidget game;
    game.setWindowTitle("Pac-Man");
    game.show();

    return app.exec();
}
