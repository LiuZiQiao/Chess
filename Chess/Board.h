#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include<QPainter>
class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);
    ~Board();
    void paintEvent(QPaintEvent*);
signals:

public slots:
};

#endif // BOARD_H
