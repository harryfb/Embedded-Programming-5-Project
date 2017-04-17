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
