/*
 * AD7705 based Photodiode Camera
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 * Copyright (c) 2013-2015  Bernd Porr <mail@berndporr.me.uk>
 * Copyright (c) 2017 Harry Bullough <2041675b@student.gla.ac.uk
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * For compliation instructions, see README.md
 */

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
