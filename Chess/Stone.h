//#ifndef STONE_H
//#define STONE_H
#include<QString>

class Stone
{
public:
    Stone();
    ~Stone();
    enum TYPE{CHE,MA,PAO,BING,JIANG,SHI,XIANG};

    int _row;
    int _col;
    TYPE _type;

    int _id;
    bool _red;
    bool _dead;

    void init(int id);

    QString getText();

};

//#endif // STONE_H
