#ifndef SIMPLEDATE_H
#define SIMPLEDATE_H


#include<QString>
#include<QDate>

struct SimpleDate
{
    void init(const QString& s);
    void init(const QDate& date);

    int y;
    int m;
    int d;



    bool inRange(SimpleDate d1,SimpleDate d2);

    bool lessEqual(SimpleDate d);
};

#endif // SIMPLEDATE_H
