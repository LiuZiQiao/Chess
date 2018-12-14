#ifndef BOARD_H
#define BOARD_H
#include"Stone.h"
#include <QWidget>

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);
    ~Board();
    Stone _s[32];

    int _r ; //棋子半径
    int _selectid;
    bool _bRedTurn;
    QPoint center(int row,int col);
    QPoint center(int id);
    void paintEvent(QPaintEvent*);
    bool getRowCol(QPoint pt,int& row,int& col);
    void drawStone(QPainter& painter,int id);//绘制棋子

    void mouseReleaseEvent(QMouseEvent *);

//    bool canMove(int moveid,int row,int col, int killid);
//    bool canMove1(int moveid,int row,int col, int killid);
//    bool canMove2(int moveid,int row,int col, int killid);
//    bool canMove3(int moveid,int row,int col, int killid);
//    bool canMove4(int moveid,int row,int col, int killid);
//    bool canMove5(int moveid,int row,int col, int killid);
//    bool canMove6(int moveid,int row,int col, int killid);
//    bool canMove7(int moveid,int row,int col, int killid);
signals:

public slots:
};

#endif // BOARD_H
