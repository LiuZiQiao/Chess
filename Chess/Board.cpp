#include "Board.h"
#include<QPainter>
#include<QMouseEvent>
#define GetRowCol(__row, __col, __id) \
    int __row = _s[__id]._row;\
    int __col = _s[__id]._col

Board::Board(QWidget *parent) : QWidget(parent)
{
    //初始化棋子位置名字
    init(true);
}

void Board::init(bool bRedSide)
{
    for(int i=0;i<32;++i)
    {
        _s[i].init(i);
    }
    if(bRedSide)
    {
        for(int i=0;i<32;++i)
        {
            _s[i].rotate();
        }
    }
    _selectid = -1;
    _bRedTurn = true;
    _bSide  = bRedSide;
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
bool Board::canMove(int moveid, int row, int col, int killid)
{
    if(_s[moveid]._red == _s[killid]._red)   // moveid 和killid颜色相同，防止自己吃自己
    {
        //更换选择
        _selectid = killid;
        update();
        return false;
    }

    switch(_s[moveid]._type)
    {
    case Stone::JIANG:
        return canMoveJIANG(moveid,row,col,killid);

    case Stone::SHI:
        return canMoveSHI(moveid,row,col,killid);

    case Stone::XIANG:
        return canMoveXIANG(moveid,row,col,killid);

    case Stone::CHE:
        return canMoveCHE(moveid,row,col,killid);

    case Stone::MA:
        return canMoveMA(moveid,row,col,killid);

     case Stone::PAO:
        return canMovePAO(moveid,row,col,killid);

     case Stone::BING:
        return canMoveBING(moveid,row,col,killid);

    }
    return true;
}
int Board::getStoneCountAtLine(int row1, int col1, int row2, int col2)
{
    int ret = 0;
    if(row1 != row2 && col1 != col2)
        return -1;
    if(row1 == row2 && col1 == col2)
        return -1;

    if(row1 == row2)
    {
        int min = col1<col2?col1:col2;
        int max = col1<col2?col2:col1;
        for(int col = min+1;col<max;++col)
        {
            if(getStoneId(row1,col)!= -1)
                ++ret;
        }
    }
    else
    {
        int min = row1<row2?row1:row2;
        int max = row1<row2?row2:row1;
        for(int col = min+1;col<max;++col)
        {
            if(getStoneId(row1,col)!= -1)
                ++ret;
        }
    }
    return ret;
}
int Board::getStoneId(int row, int col)
{
    for(int i=0;i<32;++i)
    {
        if(_s[i]._row == row && _s[i]._col == col)
            return i;
    }
    return -1;
}

int Board::relation(int row1,int col1,int row,int col)
{
    return abs(row1-row)*10+abs(col1-col);
}

bool Board::isBottomSide(int id)
{
    return _bSide == _s[id]._red;
}
//将的移动规则
bool Board::canMoveJIANG(int moveid, int row, int col, int killid)
{
    /*
     *  目标位置必须在九宫格内
     *  移动距离是一个格子
     */
    if(_s[moveid]._red)
    {
        if(row>2)       //将不能出横线的第三个线
            return false;
    }
    else
    {
        if(row<7) return false;     //将横线的第8,9,10线上移动
    }

    if(col<3) return false;         //将应该在竖线的第4~6之间移动
    if(col>5) return false;

    //移动距离是1
    int dr = _s[moveid]._row - row;
    int dc = _s[moveid]._col - col;
    int d = abs(dr)*10 + abs(dc); // 当马的时候是12, 21   象：22   将：10, 1
    if(d == 1 || d == 10)
        return true;

    return false;
}

//士移动规则
bool Board::canMoveSHI(int moveid, int row, int col, int )
{
    if(_s[moveid]._red)
    {
        if(row>2)
            return false;
    }
    else
    {
        if(row<7)
            return false;
    }

    if(col<3) return false;
    if(col>5) return false;

    int dr = _s[moveid]._row-row;
    int dc = _s[moveid]._col-col;
//    int d = abs(dr)*10+abs(dc);
    if(abs(dr)==1 && abs(dc)==1)
//    if(d == 11)
        return true;
    return false;

    //重写该函数
//    GetRowCol(row1,col1,moveid);
//    int r = relation(row1,col1,row,col);
//    if(r != 11) return false;

//    if(col <3|| col>5) return false;
//    if(isBottomSide(moveid))
//    {
//        if(row<7) return false;
//    }
//    else
//    {
//        if(row >2) return false;
//    }
//    return true;
}

//象移动规则
bool Board::canMoveXIANG(int moveid, int row, int col, int)
{
    if(_s[moveid]._red)
    {
        if(row>2)
            return false;
    }
    else
    {
        if(row<7)
            return false;
    }

    if(col<3) return false;
    if(col>5) return false;

    int dr = _s[moveid]._row-row;
    int dc = _s[moveid]._col-col;
    int d = abs(dr)*10+abs(dc);
    if(d == 11)
        return true;
    return false;


//    GetRowCol(row1, col1, moveid);
//    int r = relation(row1,col1,row,col);
//    if(r != 22) return false;

//    int rEye = (row+row1)/2;
//    int cEye = (col+col1)/2;

//    if(getStoneId(rEye,cEye)!= -1)
//        return false;
//    if(isBottomSide(moveid))
//    {
//        if(row<4) return false;
//    }
//    else
//    {
//        if(row>5) return false;
//    }

//    return true;
}



bool Board::canMoveCHE(int moveid, int row, int col, int killid)
{
    GetRowCol(row1,col1,moveid);
    int ret = getStoneCountAtLine(row1,col1,row,col);
    if(ret == 0)
        return true;
    return false;
}
bool Board::canMoveMA(int moveid, int row, int col, int)
{
    GetRowCol(row1,col1,moveid);
    int r = relation(row1,col1,row,col);
    if(r != 12 && r != 21)
        return false;
    if(r == 12)
    {
        if(getStoneId(row1,(col+col1)/2)!= -1)
            return false;
    }
    else
    {
        if(getStoneId((row+row1)/2,col1)!= -1)
            return false;
    }
    return true;
}
bool Board::canMovePAO(int moveid, int row, int col, int killid)
{
    GetRowCol(row1,col1,moveid);
    int ret = getStoneCountAtLine(row,col,row1,col1);
    if(killid != -1)
    {
        if(ret == 1) return true;
    }
    else
    {
        if(ret == 0) return true;
    }
    return false;
}
bool Board::canMoveBING(int moveid, int row, int col, int)
{
    int dr = _s[moveid]._row-row;
    int dc = _s[moveid]._col-col;
    if(_s[moveid]._red)
    {
        //过河前
        if(_s[moveid]._row>=6&&_s[moveid]._row<=7)
        {
            if(dr==1&&dc==0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        //过河后
        else
        {
           if(abs(dr)==1 && abs(dc)==0||abs(dr)==0&&abs(dc)==1)
           {
               if(dr==1&&dc==0)   //竖着走
               {
                   return true;
               }
               else
               {
                   return false;
               }
           }
           else
           {
               return false;
           }
        }

    }
    else
    {
        if(_s[moveid]._row>4 && _s[moveid]._row<=5)
        {
            if(dr==1 && dc ==0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if(abs(dr)==1 && abs(dc)==0||abs(dr)==0&&abs(dc)==1)
            {
                if(dr==-1)  //竖着走
                {
                    return true;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                return false;
            }
        }
    }
    return true;


//    GetRowCol(row1,col1,moveid);
//    int r = relation(row,col1,row,col);
//    if(r != 1 && r != 10) return false;
//    if(isBottomSide(moveid))
//    {
//        if(row>row1) return false;
//        if(row1 >=5 && row == row1) return false;
//    }
//    else
//    {
//        if(row < row1) return false;
//        if(row1 <= 4 && row == row1) return false;
//    }
//    return true;
}


void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    QPoint pt = ev->pos();  //将pt转化为行列

    int row,col;
    bool bRet = getRowCol(pt,row,col); //获取被点位置坐标
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
            if(_bRedTurn == _s[clickid]._red)
            {
                _selectid = clickid;
                update();
            }
        }
    }
    else
    {
        if(canMove(_selectid,row,col,clickid))
        {
            _s[_selectid]._row = row;
            _s[_selectid]._col = col;
            if(clickid != -1)
            {
                _s[clickid]._dead = true;
            }
            _selectid = -1;
            _bRedTurn = !_bRedTurn;
            update();
        }
    }
}

Board::~Board()
{

}

