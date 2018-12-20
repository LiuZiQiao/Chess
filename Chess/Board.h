#ifndef BOARD_H
#define BOARD_H
#include"Stone.h"
#include"Step.h"
#include <QWidget>
#include<QVector>

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
    QPoint _off;

    QVector<Step*> _steps;

    /*game status*/
    int _selectid;
    bool _bRedTurn;
    void init(bool bRedSide);
    bool red(int id);
    void killStone(int id);
    QPoint center(int row,int col);
    QPoint center(int id);

    /*draw functions */
    void paintEvent(QPaintEvent*);

//    bool getRowCol(QPoint pt,int& row,int& col);
    void drawStone(QPainter& painter,int id);//绘制棋子

    /*move stone*/
    void mouseReleaseEvent(QMouseEvent *);
    void click(int id, int row, int col);
    void click(QPoint pt);
    void moveStone(int moveid, int row, int col);
    void moveStone(int moveid, int killid, int row, int col);
    void saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps);
    bool getClickRowCol(QPoint pt, int &row, int &col);


    void tryMoveStone(int killid, int row, int col);
    void trySelectStone(int id);
    bool canSelect(int id);
    bool sameColor(int id1, int id2);


    /*rule helper functions*/
    int relation(int row1,int col1,int row,int col);    //棋子距离
    int getStoneCountAtLine(int row1,int col1,int row2,int col2); //车 炮等走直线计算
    bool isBottomSide(int id);
    int getStoneId(int row,int col);
    bool isDead(int id);

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
