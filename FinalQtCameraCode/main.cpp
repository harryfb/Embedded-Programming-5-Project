#include "window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window w;
    w.showMaximized();
    //Call the Timer Event function every 25ms
    w.startTimer(25);

    return a.exec();
}
