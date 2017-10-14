#ifndef ELE_H
#define ELE_H

#include<QString>
#include<QList>

struct Ele
{
public:
    Ele();
    void init(QString stream);

    QString cname;


    QList<double> dataList;

    double getData(int index);

    //量测源
    QString res;
};
#endif // ELE_H
