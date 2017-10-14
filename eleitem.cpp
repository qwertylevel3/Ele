#include "eleitem.h"

EleItem::EleItem()
{

}

void EleItem::setEleData(Ele e)
{
    eleData=e;

    this->setText(eleData.cname);
}

Ele EleItem::getEleData()
{
    return eleData;
}
