#include "window.h"
#include "ui_window.h"
#include "qwt/qwt_knob.h"

    int Value[5][6];
    int WindowRow = 0;
    int WindowCol = 0;

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);
    adcreader = new ADCreader();
    adcreader->start();

    knob = new QwtKnob(this);
    knob->setValue(offset);
    knob->setRange(-255, 255);
    knob->move(40, 50);
    knob->resize(150, 150);

    connect(knob, SIGNAL(valueChanged(int)), SLOT(setValue(int)));

}


void Window::setValue(int value)
{
    this->offset() = offset;
}


void Window::timerEvent( QTimerEvent * )
{

for(WindowRow = 0; WindowRow < 5; WindowRow++){
	for(WindowCol = 0; WindowCol < 6; WindowCol++){
            Value[WindowRow][WindowCol] = (adcreader->getSample(WindowRow, WindowCol));
	    Value[WindowRow][WindowCol] += offset;

	    if (Value[WindowRow][WindowCol] > 255) {
		Value[WindowRow][WindowCol] = 255;
	    }

	    if (Value[WindowRow][WindowCol] < 0) {
		Value[WindowRow][WindowCol] = 0;
	    }

	}
}
    this->update();
}

Window::~Window()
{
    delete ui;
    adcreader->quit();
    adcreader->wait();
    delete adcreader;
}

void Window::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    int i = 200;
    int i2 = 20;
    int i3 = 120;
    int i4 = 120;

    int i1res = 120;
    int i2res = 120;

    QRectF rectangle0(i,i2,i3,i4); //1st row 1st pixel
    i = i + i1res;

    QRectF rectangle1(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle2(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle3(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle4(i,i2,i3,i4);
    i = i+i1res;

    QRectF rectangle5(i,i2,i3,i4);
    i = 200;
    i2 = i2 + i2res;

    QRectF rectangle6(i,i2,i3,i4); //2nd row 1st pixel
    i = i + i1res;

    QRectF rectangle7(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle8(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle9(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle10(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle11(i,i2,i3,i4);
    i2 = i2 + i2res;
    i = 200;

    QRectF rectangle12(i,i2,i3,i4); //3rd row 1st pixel
    i = i + i1res;

    QRectF rectangle13(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle14(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle15(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle16(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle17(i,i2,i3,i4);
    i = 200;
    i2 = i2 + i2res;

    QRectF rectangle18(i,i2,i3,i4); //4th row 1st pixel
    i = i + i1res;

    QRectF rectangle19(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle20(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle21(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle22(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle23(i,i2,i3,i4);
    i = 200;
    i2 = i2 + i2res;

    QRectF rectangle24(i,i2,i3,i4); //5th row 1st pixel
    i = i + i1res;

    QRectF rectangle25(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle26(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle27(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle28(i,i2,i3,i4);
    i = i + i1res;

    QRectF rectangle29(i,i2,i3,i4);


        QRgb BrushColour0 = (Value[0][0] << 16) + (Value [0][0] << 8) + Value[0][0];
        QRgb BrushColour1 = (Value[0][1] << 16) + (Value [0][1] << 8) + Value[0][1];
        QRgb BrushColour2 = (Value[0][2] << 16) + (Value [0][2] << 8) + Value[0][2];
        QRgb BrushColour3 = (Value[0][3] << 16) + (Value [0][3] << 8) + Value[0][3];
        QRgb BrushColour4 = (Value[0][4] << 16) + (Value [0][4] << 8) + Value[0][4];
        QRgb BrushColour5 = (Value[0][5] << 16) + (Value [0][5] << 8) + Value[0][5];
        QRgb BrushColour6 = (Value[1][0] << 16) + (Value [1][0] << 8) + Value[1][0];
        QRgb BrushColour7 = (Value[1][1] << 16) + (Value [1][1] << 8) + Value[1][1];
        QRgb BrushColour8 = (Value[1][2] << 16) + (Value [1][2] << 8) + Value[1][2];
        QRgb BrushColour9 = (Value[1][3] << 16) + (Value [1][3] << 8) + Value[1][3];
        QRgb BrushColour10 = (Value[1][4] << 16) + (Value [1][4] << 8) + Value[1][4];
        QRgb BrushColour11 = (Value[1][5] << 16) + (Value [1][5] << 8) + Value[1][5];
        QRgb BrushColour12 = (Value[2][0] << 16) + (Value [2][0] << 8) + Value[2][0];
        QRgb BrushColour13 = (Value[2][1] << 16) + (Value [2][1] << 8) + Value[2][1];
        QRgb BrushColour14 = (Value[2][2] << 16) + (Value [2][2] << 8) + Value[2][2];
        QRgb BrushColour15 = (Value[2][3] << 16) + (Value [2][3] << 8) + Value[2][3];
        QRgb BrushColour16 = (Value[2][4] << 16) + (Value [2][4] << 8) + Value[2][4];
        QRgb BrushColour17 = (Value[2][5] << 16) + (Value [2][5] << 8) + Value[2][5];
        QRgb BrushColour18 = (Value[3][0] << 16) + (Value [3][0] << 8) + Value[3][0];
        QRgb BrushColour19 = (Value[3][1] << 16) + (Value [3][1] << 8) + Value[3][1];
        QRgb BrushColour20 = (Value[3][2] << 16) + (Value [3][2] << 8) + Value[3][2];
        QRgb BrushColour21 = (Value[3][3] << 16) + (Value [3][3] << 8) + Value[3][3];
        QRgb BrushColour22 = (Value[3][4] << 16) + (Value [3][4] << 8) + Value[3][4];
        QRgb BrushColour23 = (Value[3][5] << 16) + (Value [3][5] << 8) + Value[3][5];
        QRgb BrushColour24 = (Value[4][0] << 16) + (Value [4][0] << 8) + Value[4][0];
        QRgb BrushColour25 = (Value[4][1] << 16) + (Value [4][1] << 8) + Value[4][1];
        QRgb BrushColour26 = (Value[4][2] << 16) + (Value [4][2] << 8) + Value[4][2];
        QRgb BrushColour27 = (Value[4][3] << 16) + (Value [4][3] << 8) + Value[4][3];
        QRgb BrushColour28 = (Value[4][4] << 16) + (Value [4][4] << 8) + Value[4][4];
        QRgb BrushColour29 = (Value[4][5] << 16) + (Value [4][5] << 8) + Value[4][5];

        QBrush brush0(BrushColour0,Qt::SolidPattern);
        QBrush brush1(BrushColour1,Qt::SolidPattern);
        QBrush brush2(BrushColour2,Qt::SolidPattern);
        QBrush brush3(BrushColour3,Qt::SolidPattern);
        QBrush brush4(BrushColour4,Qt::SolidPattern);
        QBrush brush5(BrushColour5,Qt::SolidPattern);
        QBrush brush6(BrushColour6,Qt::SolidPattern);
        QBrush brush7(BrushColour7,Qt::SolidPattern);
        QBrush brush8(BrushColour8,Qt::SolidPattern);
        QBrush brush9(BrushColour9,Qt::SolidPattern);
        QBrush brush10(BrushColour10,Qt::SolidPattern);
        QBrush brush11(BrushColour11,Qt::SolidPattern);
        QBrush brush12(BrushColour12,Qt::SolidPattern);
        QBrush brush13(BrushColour13,Qt::SolidPattern);
        QBrush brush14(BrushColour14,Qt::SolidPattern);
        QBrush brush15(BrushColour15,Qt::SolidPattern);
        QBrush brush16(BrushColour16,Qt::SolidPattern);
        QBrush brush17(BrushColour17,Qt::SolidPattern);
        QBrush brush18(BrushColour18,Qt::SolidPattern);
        QBrush brush19(BrushColour19,Qt::SolidPattern);
        QBrush brush20(BrushColour20,Qt::SolidPattern);
        QBrush brush21(BrushColour21,Qt::SolidPattern);
        QBrush brush22(BrushColour22,Qt::SolidPattern);
        QBrush brush23(BrushColour23,Qt::SolidPattern);
        QBrush brush24(BrushColour24,Qt::SolidPattern);
        QBrush brush25(BrushColour25,Qt::SolidPattern);
        QBrush brush26(BrushColour26,Qt::SolidPattern);
        QBrush brush27(BrushColour27,Qt::SolidPattern);
        QBrush brush28(BrushColour28,Qt::SolidPattern);
        QBrush brush29(BrushColour29,Qt::SolidPattern);

        painter.fillRect(rectangle0,brush0);
        //painter.drawRect(rectangle0);

        painter.fillRect(rectangle1,brush1);
        //painter.drawRect(rectangle1);

        painter.fillRect(rectangle2,brush2);
        //painter.drawRect(rectangle2);

        painter.fillRect(rectangle3,brush3);
        //painter.drawRect(rectangle3);

        painter.fillRect(rectangle4,brush4);
        //painter.drawRect(rectangle4);

        painter.fillRect(rectangle5,brush5);
        //painter.drawRect(rectangle5);

        painter.fillRect(rectangle6,brush6);
        //painter.drawRect(rectangle6);

        painter.fillRect(rectangle7,brush7);
        //painter.drawRect(rectangle7);

        painter.fillRect(rectangle8,brush8);
        //painter.drawRect(rectangle8);

        painter.fillRect(rectangle9,brush9);
        //painter.drawRect(rectangle9);

        painter.fillRect(rectangle10,brush10);
        //painter.drawRect(rectangle10);

        painter.fillRect(rectangle11,brush11);
        //painter.drawRect(rectangle11);

        painter.fillRect(rectangle12,brush12);
        //painter.drawRect(rectangle12);

        painter.fillRect(rectangle13,brush13);
        //painter.drawRect(rectangle13);

        painter.fillRect(rectangle14,brush14);
        //painter.drawRect(rectangle14);

        painter.fillRect(rectangle15,brush15);
        //painter.drawRect(rectangle15);

        painter.fillRect(rectangle16,brush16);
        //painter.drawRect(rectangle16);

        painter.fillRect(rectangle17,brush17);
        //painter.drawRect(rectangle17);

        painter.fillRect(rectangle18,brush18);
        //painter.drawRect(rectangle18);

        painter.fillRect(rectangle19,brush19);
        //painter.drawRect(rectangle19);

        painter.fillRect(rectangle20,brush20);
        //painter.drawRect(rectangle20);

        painter.fillRect(rectangle21,brush21);
        //painter.drawRect(rectangle21);

        painter.fillRect(rectangle22,brush22);
        //painter.drawRect(rectangle22);

        painter.fillRect(rectangle23,brush23);
        //painter.drawRect(rectangle23);

        painter.fillRect(rectangle24,brush24);
        //painter.drawRect(rectangle24);

        painter.fillRect(rectangle25,brush25);
        //painter.drawRect(rectangle25);

        painter.fillRect(rectangle26,brush26);
        //painter.drawRect(rectangle26);

        painter.fillRect(rectangle27,brush27);
        //painter.drawRect(rectangle27);

        painter.fillRect(rectangle28,brush28);
        //painter.drawRect(rectangle28);

        painter.fillRect(rectangle29,brush29);
        //painter.drawRect(rectangle29);
}


