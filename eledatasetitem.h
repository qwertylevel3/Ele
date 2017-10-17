#ifndef ELEDATASETITEM_H
#define ELEDATASETITEM_H

#include<QString>
#include<QStandardItem>

class EleDataSetItem:public QStandardItem
{
public:
    EleDataSetItem();
    QString getFilePath();
    void setFilePath(QString path);

    QString getDataName();
    void setDataName(QString name);
protected:
    QString filepath;
    QString dataName;
};

#endif // ELEDATASETITEM_H
