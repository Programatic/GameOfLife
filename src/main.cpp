#include <QApplication>
#include "../include/gamewindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GameWindow gw;
    gw.show();

    return app.exec();
}
