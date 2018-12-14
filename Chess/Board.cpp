#include "Board.h"
#include<QPainter>
#include<QMouseEvent>
Board::Board(QWidget *parent) : QWidget(parent)
{
    //初始化棋子位置名字
    for(int i=0;i<32;++i)
    {
        _s[i].init(i);
    }
    _selectid = -1;
}

void Board::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int d = 40;
    _r = d/2;
    //横线
    for(int i=1;i<=10;++i)
    {
        painter.drawLine(QPoint(d,d*i),QPoint(9*d,i*d));
    }
    //竖线
    for(int i=1;i<=9;++i)
    {
        if(i==1||i==9)
        {
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,10*d));

        }
        else{
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,5*d));
            painter.drawLine(QPoint(i*d,6*d),QPoint(i*d,10*d));
        }
    }
    //对角线
    painter.drawLine(QPoint(4*d,1*d),QPoint(6*d,3*d));
    painter.drawLine(QPoint(6*d,1*d),QPoint(4*d,3*d));

    painter.drawLine(QPoint(4*d,10*d),QPoint(6*d,8*d));
    painter.drawLine(QPoint(4*d,8*d),QPoint(6*d,10*d));

    //画32个棋子
    for(int i=0;i<32;++i)
    {
        drawStone(painter,i);
    }
}
QPoint Board::center(int col,int row)
{
    QPoint ret;
    ret.rx() = (row+1)*_r*2;
    ret.ry() = (col+1)*_r*2;

    return ret;
}

QPoint Board::center(int id)
{
    return center(_s[id]._row,_s[id]._col);
}


void Board::drawStone(QPainter& painter, int id)
{
    if(_s[id]._dead)
        return;

    QPoint c = center(id);
    QRect rect = QRect(c.x()-_r,c.y()-_r,_r*2,_r*2);
    if(_selectid == id)
    {
        painter.setBrush(QBrush(Qt::gray));
    }
    else
    {
        painter.setBrush(QBrush(Qt::yellow));
    }
    painter.setPen(Qt::black);

    painter.drawEllipse(center(id),_r,_r);


    if(_s[id]._red){
        painter.setPen(Qt::red);
    }
    painter.setFont(QFont("system",_r,700));
    painter.drawText(rect,_s[id].getText(),QTextOption(Qt::AlignCenter));
}

bool Board::getRowCol(QPoint pt, int &row, int &col)
{
    for(row=0;row<=9;++row)
    {
        for(col=0;col<=8;++col)
        {
            QPoint c = center(row,col);
            int dx = c.x()-pt.x();
            int dy = c.y()-pt.y();
            int dist = dx*dx+dy*dy;
            if(dist<_r*_r)
                return true;
        }
    }
    return false;
}
//bool Board::canMove(int moveid, int row, int col, int killid)
//{
//    if(_s[moveid]._red == _s[killid]._red)   // moveid 和killid颜色相同
//    {
//        //更换选择
//        _selectid = killid;
//        update();
//        return false;
//    }

//    switch(_s[moveid]._type)
//    {
//    case Stone::JIANG:
//        return canMove1(moveid,row,col,killid);
//        break;
//    case Stone::SHI:
//        return canMove2(moveid,row,col,killid);
//        break;
//    case Stone::XIANG:
//        return canMove3(moveid,row,col,killid);
//        break;
//    case Stone::CHE:
//        return canMove4(moveid,row,col,killid);
//        break;
//    case Stone::MA:
//        return canMove5(moveid,row,col,killid);
//        break;
//     case Stone::PAO:
//        return canMove6(moveid,row,col,killid);
//        break;
//     case Stone::BING:
//        return canMove7(moveid,row,col,killid);
//        break;
//    }
//    return true;
//}

//bool Board::canMove1(int moveid, int row, int col, int killid)
//{
//    /*
//     *  目标位置必须在九宫格内
//     *  移动距离是一个格子
//     */
//    if(_s[moveid]._red)
//    {
//        if(row>2) return false;
//    }
//    else
//    {
//        if(row<7) return false;
//    }

//    if(col<3) return false;
//    if(col>5) return false;

//    int dr = _s[moveid]._row - row;
//    int dc = _s[moveid]._col - col;
//    int d = abs(dr)*10 + abs(dc); // 12, 21   22   10, 1
//    if(d == 1 || d == 10)
//        return true;

//    return false;
//}

//bool Board::canMove2(int moveid, int row, int col, int killid)
//{
//    if(_s[moveid]._red)
//    {
//        if(row>2)
//            return false;
//    }
//    else
//    {
//        if(row<7)
//            return false;
//    }

//    if(col<3) return false;
//    if(col>5) return false;

//    int dr = _s[moveid]._row-row;
//    int dc = _s[moveid]._col-col;
//    int d = abs(dr)*10+abs(dc);
//    if(d == 11)
//        return true;
//    return false;
//}
//bool Board::canMove3(int moveid, int row, int col, int killid)
//{
//    return true;
//}
//bool Board::canMove4(int moveid, int row, int col, int killid)
//{
//    return true;
//}
//bool Board::canMove5(int moveid, int row, int col, int killid)
//{
//    return true;
//}
//bool Board::canMove6(int moveid, int row, int col, int killid)
//{
//    return true;
//}
//bool Board::canMove7(int moveid, int row, int col, int killid)
//{
//    return true;
//}


void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    QPoint pt = ev->pos();  //将pt转化为行列

    int row,col;
    bool bRet = getRowCol(pt,row,col);
    if(bRet == false) //点到棋盘外
        return;
    int i;
    int clickid = -1;
    for(i=0;i<32;++i)
    {
        if(_s[i]._row == row && _s[i]._col == col && _s[i]._dead ==false)
        {
            break;
        }
    }
    if(i<32)
    {
        clickid = i;
//        update();
    }

    if(_selectid == -1)
    {
        if(clickid != -1)
        {

                _selectid = clickid;
                update();
        }
    }
    else
    {
//        if(canMove(_selectid,row,col,clickid))
//        {
            _s[_selectid]._row = row;
            _s[_selectid]._col = col;
            if(clickid != -1)
            {
                _s[clickid]._dead = true;
            }
            _selectid = -1;
//            _bRedTurn = !_bRedTurn;
            update();
//        }
    }
}

Board::~Board()
{

}

