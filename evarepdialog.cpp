#include "evarepdialog.h"
#include "ui_evarepdialog.h"
#include"util.h"
#include"evarepdialog.h"
#include<QSqlRecord>
#include"repdialog.h"

EvaRepDialog::EvaRepDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::EvaRepDialog)
{
    ui->setupUi(this);

    connect(ui->evaButton,SIGNAL(clicked(bool)),this,SLOT(generateRep()));
}

EvaRepDialog::~EvaRepDialog()
{
    delete ui;
}

void EvaRepDialog::setModel(QSqlTableModel *m)
{
    dataModel=m;

    initListWidget();
    initHeaderTable();
}

void EvaRepDialog::generateRep()
{
    //clear
    dataModel->select();


    //get all ele name
    QList<QString> eleNameList;

    for(int i=0;i<ui->eleTable->rowCount();i++)
    {
        if(ui->eleTable->item(i,0)->checkState()==Qt::Checked)
        {
            eleNameList.push_back(ui->eleTable->item(i,1)->text());
        }
    }

    //get data range
    QDate date1=ui->dateEdit1->date();
    QDate date2=ui->dateEdit2->date();

    SimpleDate d1;
    SimpleDate d2;

    d1.init(date1);
    d2.init(date2);


    QStandardItemModel* model=new QStandardItemModel();


    //生成表头
    generateHeader(model);

    //计算每个待评价机组表项目

    for(int i=0;i<eleNameList.size();i++)
    {
        //        double value=evaluate(eleNameList.at(i),d1,d2);
        QList<QStandardItem*> row;
        row.append(new QStandardItem(eleNameList[i]));

        int j=0;
        for(;j<ui->headerTable->rowCount()-1;j++)
        {
            //处理贡献电量表项
            if(ui->headerTable->item(j,0)->checkState()==Qt::Checked)
            {
                double value=getTableValue(eleNameList[i],ui->headerTable->item(j,1)->text(),d1,d2);
                row.append(new QStandardItem(QString::number(value)));
            }
        }
        if(ui->headerTable->item(j,0)->checkState()==Qt::Checked)
        {
            //处理评价指数表项
            double value=evaluate(eleNameList[i],d1,d2);
            row.append(new QStandardItem(QString::number(value)));
        }
        model->appendRow(row);
    }

    RepDialog dialog;
    dialog.setModel(model);
    dialog.exec();
}

void EvaRepDialog::initHeaderTable()
{
    dataModel->select();

    QList<QString> headerList;
    headerList.append("15秒实际贡献电量");
    headerList.append("15秒理论贡献电量");
    headerList.append("30秒实际贡献电量");
    headerList.append("30秒理论贡献电量");
    headerList.append("45秒实际贡献电量");
    headerList.append("45秒理论贡献电量");
    headerList.append("60秒实际贡献电量");
    headerList.append("60秒理论贡献电量");
    headerList.append("评价指数");


    auto iter=headerList.begin();
    int count=0;
    while(iter!=headerList.end())
    {
        QTableWidgetItem* checkBoxItem=new QTableWidgetItem();
        checkBoxItem->setCheckState(Qt::Unchecked);

        QTableWidgetItem* item=new QTableWidgetItem();
        item->setText(*iter);

        ui->headerTable->insertRow(count);

        ui->headerTable->setItem(count,0,checkBoxItem);
        ui->headerTable->setItem(count,1,item);
        iter++;
        count++;
    }
}

void EvaRepDialog::initListWidget()
{
    dataModel->select();

    QSet<QString> tag;
    for(int i=0;i<dataModel->rowCount();i++)
    {
        QString name=(dataModel->record(i)).value("机组名称").toString();
        if(!tag.contains(name))
        {
            tag.insert(name);
        }
    }

    int count=0;
    auto iter=tag.begin();
    while(iter!=tag.end())
    {
        QTableWidgetItem* checkBoxItem=new QTableWidgetItem();
        checkBoxItem->setCheckState(Qt::Unchecked);

        QTableWidgetItem* item=new QTableWidgetItem();
        item->setText(*iter);

        ui->eleTable->insertRow(count);

        ui->eleTable->setItem(count,0,checkBoxItem);
        ui->eleTable->setItem(count,1,item);

        iter++;
        count++;
    }

}

double EvaRepDialog::evaluate(const QString &eleName, SimpleDate d1, SimpleDate d2)
{
    double result=0.0;
    int count=0;

    for(int i=0;i<dataModel->rowCount();i++)
    {
        QString name=(dataModel->record(i)).value("机组名称").toString();
        if(name!=eleName)
        {
            continue;
        }

        QString dateTime=(dataModel->record(i)).value("响应时间").toString();
        QString date=(dateTime.split(" "))[0];

        SimpleDate d;
        d.init(date);

        if(d.inRange(d1,d2))
        {
            double r15=(dataModel->record(i)).value("15秒实际贡献电量").toDouble();
            double t15=(dataModel->record(i)).value("15秒理论贡献电量").toDouble();
            double r30=(dataModel->record(i)).value("30秒实际贡献电量").toDouble();
            double t30=(dataModel->record(i)).value("15秒理论贡献电量").toDouble();
            double r45=(dataModel->record(i)).value("45秒实际贡献电量").toDouble();
            double t45=(dataModel->record(i)).value("15秒理论贡献电量").toDouble();
            double r60=(dataModel->record(i)).value("60秒实际贡献电量").toDouble();
            double t60=(dataModel->record(i)).value("15秒理论贡献电量").toDouble();

            double k1=ui->k1SpinBox->value();
            double k2=ui->k2SpinBox->value();
            double k3=ui->k3SpinBox->value();
            double k4=ui->k4SpinBox->value();

            double tempValue=Util::evaluate(
                                    k1,k2,k3,k4,
                                    r15,t15,r30,t30,r45,t45,r60,t60);

            result+=tempValue;

            count++;
        }
    }
    return count==0?0:(result/count);
}

double EvaRepDialog::getTableValue(const QString &eleName, const QString &header, SimpleDate d1, SimpleDate d2)
{
    double result=0.0;
    int count=0;

    for(int i=0;i<dataModel->rowCount();i++)
    {
        QString name=(dataModel->record(i)).value("机组名称").toString();
        if(name!=eleName)
        {
            continue;
        }

        QString dateTime=(dataModel->record(i)).value("响应时间").toString();
        QString date=(dateTime.split(" "))[0];

        SimpleDate d;
        d.init(date);

        if(d.inRange(d1,d2))
        {
            double tempValue=(dataModel->record(i)).value(header).toDouble();

            result+=tempValue;

            count++;
        }
    }
    return count==0?0:(result/count);
}

void EvaRepDialog::generateHeader(QStandardItemModel *model)
{
    QStringList header;
    header.push_back("机组名称");

    for(int i=0;i<ui->headerTable->rowCount();i++)
    {
        if(ui->headerTable->item(i,0)->checkState()==Qt::Checked)
        {
            header.push_back(ui->headerTable->item(i,1)->text());
        }
    }

    model->setHorizontalHeaderLabels(header);
}
