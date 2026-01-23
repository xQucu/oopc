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

// TODO
//  reset lives and score when lives =-1
//  base-walkable doors
//  ghosts starting at base
//  ghosts scatter mode
//  ghosts frighten mode improvements
//  ghosts eaten mode

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
    int mouthAngle; // For animation

public:
    Player(const QPointF& pos, int size)
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

        // Animate mouth: opens and closes (0-20 degrees)
        int mouthDegrees = (mouthAngle % 10) * 2;

        // Get starting angle based on direction Pac-Man is facing
        // SWITCHED: 0=right(0°), 1=up(90°), 2=left(180°), 3=down(270°)
        int startAngle = -mouthDegrees;
        switch (direction) {
        case 0: // Right
            startAngle = 0;
            break;
        case 1: // Up (switched from down)
            startAngle = 270;
            break;
        case 2: // Left
            startAngle = 180;
            break;
        case 3: // Down (switched from up)
            startAngle = 90;
            break;
        }

        // Draw Pac-Man with mouth opening in direction of movement
        painter.drawPie(QRectF(position.x() + margin, position.y() + margin,
                              cellSize - 2 * margin, cellSize - 2 * margin),
                       startAngle * 16, (360 - mouthDegrees) * 16);

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

        // Animate mouth
        mouthAngle = (mouthAngle + 2) % 15;

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
        return cell != '#';
    }

    int countAvailableDirections(const QVector<QString>& map)
    {
        // Count how many directions are passable from current position
        // This helps detect intersections (3+ directions available)
        int currentX = getGridX();
        int currentY = getGridY();
        int count = 0;

        // Check all 4 directions
        // Right
        if (canMove(currentX + 1, currentY, map)) count++;
        // Down
        if (canMove(currentX, currentY + 1, map)) count++;
        // Left
        if (canMove(currentX - 1, currentY, map)) count++;
        // Up
        if (canMove(currentX, currentY - 1, map)) count++;

        return count;
    }

    bool isAtIntersection(const QVector<QString>& map)
    {
        // An intersection is where 3 or more directions are available
        return countAvailableDirections(map) >= 3;
    }

    void move(const QVector<QString>& map, int)
    {
        // Only set new target when reached current target
        if (!isAtTarget())
            return;

        int currentX = getGridX();
        int currentY = getGridY();
        int directionToUse = direction;

        // Only allow direction changes at intersections (3+ available paths)
        // At non-intersections, continue in current direction
        if (isAtIntersection(map)) {
            // At an intersection, try to apply the desired direction
            int testX = currentX;
            int testY = currentY;

            switch (nextDirection) {
            case 0: testX++; break;
            case 1: testY++; break;
            case 2: testX--; break;
            case 3: testY--; break;
            }

            // If desired direction is valid, use it
            if (canMove(testX, testY, map)) {
                directionToUse = nextDirection;
                direction = nextDirection;
            }
            // Otherwise, continue in current direction
        }
        // At non-intersections, continue in current direction (directionToUse stays as direction)

        // Move in the determined direction
        int nextX = currentX;
        int nextY = currentY;

        switch (directionToUse) {
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
        mouthAngle = 0;
    }
};

// ============================================================================
// Helper function for ghost direction selection
// ============================================================================
// When ghosts have equal distance in x and y directions, prefer: up, right, down, left
int selectPreferredDirection(int dx, int dy)
{
    if (abs(dx) > abs(dy)) {
        return (dx > 0) ? 0 : 2;  // right or left
    } else if (abs(dy) > abs(dx)) {
        return (dy > 0) ? 1 : 3;  // down or up
    } else {  // tie: abs(dx) == abs(dy)
        // Prefer in order: right (0), down (1), left (2), up (3)
        if (dx > 0) return 0;      // right
        if (dy > 0) return 1;      // down
        if (dx < 0) return 2;      // left
        return 3;                  // up
    }
}

// ============================================================================
// Helper function to get opposite direction
// ============================================================================
int getOppositeDirection(int direction)
{
    // 0=right, 1=down, 2=left, 3=up
    // Opposite of right (0) is left (2)
    // Opposite of down (1) is up (3)
    // Opposite of left (2) is right (0)
    // Opposite of up (3) is down (1)
    return (direction + 2) % 4;
}

// ============================================================================
// Enemy base class
// ============================================================================
class Enemy : public GameObject {
private:
    int direction;
    int desiredDirection;
    QColor color;
    bool frightened;
    QPointF startPosition;
    int moveCounter;

public:
    Enemy(const QPointF& pos, int size, const QColor& c)
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

    // Getters and setters
    int getDirection() const { return direction; }
    void setDirection(int d) { direction = d; }
    int getDesiredDirection() const { return desiredDirection; }
    void setDesiredDirection(int d) { desiredDirection = d; }
    int getMoveCounter() const { return moveCounter; }
    void incrementMoveCounter() { moveCounter++; }
    void resetMoveCounter() { moveCounter = 0; }
    int getGhostGridX() const { return qRound(position.x() / cellSize); }
    int getGhostGridY() const { return qRound(position.y() / cellSize); }

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
        painter.drawEllipse(position.x() + cellSize / 3.0, position.y() + cellSize / 3.0,
            eyeSize, eyeSize);
        painter.drawEllipse(position.x() + 2.0 * cellSize / 3.0 - eyeSize, position.y() + cellSize / 3.0,
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
        return cell != '#';
    }

    int countAvailableDirections(const QVector<QString>& map)
    {
        // Count how many directions are passable from current position
        // This helps detect intersections (3+ directions available)
        int currentX = getGridX();
        int currentY = getGridY();
        int count = 0;

        // Check all 4 directions
        // Right
        if (canMove(currentX + 1, currentY, map)) count++;
        // Down
        if (canMove(currentX, currentY + 1, map)) count++;
        // Left
        if (canMove(currentX - 1, currentY, map)) count++;
        // Up
        if (canMove(currentX, currentY - 1, map)) count++;

        return count;
    }

    bool isAtIntersection(const QVector<QString>& map)
    {
        // An intersection is where 3 or more directions are available
        return countAvailableDirections(map) >= 3;
    }

    void move(const QVector<QString>& map, int)
    {
        // Only set new target when reached current target
        if (!isAtTarget())
            return;

        incrementMoveCounter();

        int currentX = getGridX();
        int currentY = getGridY();
        int newX = currentX;
        int newY = currentY;

        // Only allow direction changes at intersections (3+ available paths)
        // At non-intersections, continue in current direction
        bool atIntersection = isAtIntersection(map);

        int directionToUse = getDirection();

        if (atIntersection) {
            // At an intersection, try to apply the AI's desired direction
            int desiredDir = getDesiredDirection();
            int oppositeDir = getOppositeDirection(getDirection());
            int testX = currentX;
            int testY = currentY;

            switch (desiredDir) {
            case 0: testX++; break;
            case 1: testY++; break;
            case 2: testX--; break;
            case 3: testY--; break;
            }

            // If desired direction is valid and not a turn-around, use it
            if (canMove(testX, testY, map) && desiredDir != oppositeDir) {
                directionToUse = desiredDir;
                setDirection(directionToUse);
            } else {
                // Desired direction blocked, try current direction
                testX = currentX;
                testY = currentY;

                switch (getDirection()) {
                case 0: testX++; break;
                case 1: testY++; break;
                case 2: testX--; break;
                case 3: testY--; break;
                }

                if (canMove(testX, testY, map)) {
                    directionToUse = getDirection();
                } else {
                    // Both blocked, try directions in preference order: up, right, down, left
                    // Skip the opposite direction to prevent turn-arounds
                    bool found = false;
                    int preferredOrder[] = {3, 0, 1, 2};
                    int oppositeDir = getOppositeDirection(getDirection());

                    for (int i = 0; i < 4; i++) {
                        int direction = preferredOrder[i];
                        
                        // Skip the opposite direction (no turn-arounds)
                        if (direction == oppositeDir) continue;
                        
                        testX = currentX;
                        testY = currentY;

                        switch (direction) {
                        case 0: testX++; break;
                        case 1: testY++; break;
                        case 2: testX--; break;
                        case 3: testY--; break;
                        }

                        if (canMove(testX, testY, map)) {
                            directionToUse = direction;
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        // Fallback: try random directions, but skip opposite direction
                        for (int attempt = 0; attempt < 10; attempt++) {
                            int randomDir = QRandomGenerator::global()->bounded(4);
                            
                            // Skip the opposite direction
                            if (randomDir == oppositeDir) continue;
                            
                            testX = currentX;
                            testY = currentY;

                            switch (randomDir) {
                            case 0: testX++; break;
                            case 1: testY++; break;
                            case 2: testX--; break;
                            case 3: testY--; break;
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

        // Move in the determined direction
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
            // Path blocked - try to find any valid direction (shouldn't happen in well-designed maze)
            bool found = false;
            int preferredOrder[] = {3, 0, 1, 2};
            int oppositeDir = getOppositeDirection(getDirection());

            for (int i = 0; i < 4; i++) {
                int direction = preferredOrder[i];
                
                // Skip the opposite direction
                if (direction == oppositeDir) continue;
                
                newX = currentX;
                newY = currentY;

                switch (direction) {
                case 0: newX++; break;
                case 1: newY++; break;
                case 2: newX--; break;
                case 3: newY--; break;
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
                    
                    // Skip the opposite direction
                    if (randomDir == oppositeDir) continue;
                    
                    newX = currentX;
                    newY = currentY;

                    switch (randomDir) {
                    case 0: newX++; break;
                    case 1: newY++; break;
                    case 2: newX--; break;
                    case 3: newY--; break;
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

    void setFrightened(bool f) { frightened = f; }

    bool isFrightened() const { return frightened; }

    void reset()
    {
        position = startPosition;
        targetPosition = startPosition;
        setDirection(QRandomGenerator::global()->bounded(4));
        setFrightened(false);
        resetMoveCounter();
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
// Blinky - chases player directly (Red ghost)
// ============================================================================
class Blinky : public Enemy {
public:
    Blinky(const QPointF& pos, int size)
        : Enemy(pos, size, QColor(255, 0, 0))
    {
    }

    void updateDirection(const Player& player, const QVector<QString>&) override
    {
        // Move in a random direction
        int randomDirection = QRandomGenerator::global()->bounded(4);
        setDesiredDirection(randomDirection);
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
            QApplication::quit();
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

        enemies.push_back(new Blinky(QPointF(13 * cellSize, 11 * cellSize), cellSize));

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
