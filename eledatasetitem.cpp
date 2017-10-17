#include "eledatasetitem.h"

EleDataSetItem::EleDataSetItem()
{

}

QString EleDataSetItem::getFilePath()
{
    return filepath;
}

void EleDataSetItem::setFilePath(QString path)
{
    filepath=path;
}

QString EleDataSetItem::getDataName()
{
    return dataName;
}

void EleDataSetItem::setDataName(QString name)
{
    dataName=name;
    this->setText(name);
}
