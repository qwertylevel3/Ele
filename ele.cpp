#include "ele.h"

Ele::Ele()
{

}


void Ele::init(QString stream)
{
    QStringList stringList=stream.split(",");

    cname=stringList[1];

    for(int i=2;i<stringList.size()-1;i++)
    {
        QString str=stringList[i];
        double data=str.toDouble();
        dataList.push_back(data);
    }
    res=stringList.at(stringList.size()-1);
}

double Ele::getData(int index)
{
    if(index<0 || index>= dataList.size())
    {
        return -1;
    }
    return dataList[index];

}

