#include "ConsoleWindow.h"

#include <QApplication>
#include <pacman.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PacmanWindow w;
    w.show();
    return a.exec();
}
