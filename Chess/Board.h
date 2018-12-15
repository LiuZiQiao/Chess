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

    /* game data*/
    Stone _s[32];
    int _r ; //棋子半径
    bool _bSide;

    /*game status*/
    int _selectid;
    bool _bRedTurn;
    void init(bool bRedSide);

    QPoint center(int row,int col);
    QPoint center(int id);

    /*draw functions */
    void paintEvent(QPaintEvent*);

    bool getRowCol(QPoint pt,int& row,int& col);
    void drawStone(QPainter& painter,int id);//绘制棋子


    void mouseReleaseEvent(QMouseEvent *);

    /*rule helper functions*/
    int relation(int row1,int col1,int row,int col);    //棋子距离
    int getStoneCountAtLine(int row1,int col1,int row2,int col2); //车 炮等走直线计算
    bool isBottomSide(int id);
    int getStoneId(int row,int col);

    /*rule */
    bool canMove(int moveid,int row,int col, int killid);
    bool canMoveCHE(int moveid,int row,int col, int killid);
    bool canMoveMA(int moveid,int row,int col, int killid);
    bool canMovePAO(int moveid,int row,int col, int killid);
    bool canMoveJIANG(int moveid,int row,int col, int killid);
    bool canMoveBING(int moveid,int row,int col, int killid);
    bool canMoveSHI(int moveid,int row,int col, int killid);
    bool canMoveXIANG(int moveid,int row,int col, int killid);

signals:

public slots:
};

#endif // BOARD_H
