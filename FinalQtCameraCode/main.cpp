#include "window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window w;
    w.showMaximized();

    w.startTimer(25);

    return a.exec();
}
