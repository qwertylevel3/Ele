#ifndef ELEITEM_H
#define ELEITEM_H

#include<QStandardItem>

#include"ele.h"

class EleItem:public QStandardItem
{
public:
    EleItem();
    void setEleData(Ele e);
    Ele getEleData();
protected:
    Ele eleData;
};

#endif // ELEITEM_H
