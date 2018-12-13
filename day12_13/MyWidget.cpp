#include "MyWidget.h"
#include<QPainter>
#include<QMouseEvent>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent)
{
  _button = new QPushButton("This is button",this);
}

void MyWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

//    painter.drawLine(0,0,100,100);
//    painter.drawEllipse(QPoint(100,100),10,10);
    painter.drawEllipse(_ptClick,10,10);
}

void MyWidget::mousePressEvent(QMouseEvent* ev)
{
    _ptClick = ev->pos();
    update();
}

MyWidget::~MyWidget()
{

}
