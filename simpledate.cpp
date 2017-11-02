#include "simpledate.h"

void SimpleDate::init(const QString& s)
{
    QStringList slist=s.split("/");

    y=slist[0].toInt();
    m=slist[1].toInt();
    d=slist[2].toInt();
}

void SimpleDate::init(const QDate &date)
{
    y=date.year();
    m=date.month();
    d=date.day();
}

bool SimpleDate::inRange(SimpleDate d1, SimpleDate d2)
{
    if(d1.lessEqual(*this) && this->lessEqual(d2))
    {
        return true;
    }
    return false;
}

bool SimpleDate::lessEqual(SimpleDate date)
{
    if(y<date.y)
    {
        return true;
    }
    else if(y==date.y)
    {
        if(m<date.m)
        {
            return true;
        }
        else if(m==date.m)
        {
            if(d<=date.d)
            {
                return true;
            }
        }
    }
    return false;
}
