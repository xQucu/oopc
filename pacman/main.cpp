#include "GameWidget.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    GameWidget game;
    game.setWindowTitle("Pac-Man");
    game.show();

    return app.exec();
}
