#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QtGui>
#include <QtCore>
#include <qwt/qwt_knob.h>
#include <QBoxLayout>
#include <QObject>

#include "adcreader.h"

namespace Ui {
class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();
    void timerEvent( QTimerEvent * );

protected:
    void paintEvent(QPaintEvent *e);

public slots:
    void setValue(int value);

private:
    Ui::Window *ui;

    ADCreader *adcreader;

    QwtKnob *knob;
    int offset;
};

#endif // WINDOW_H
