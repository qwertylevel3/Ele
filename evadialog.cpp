#include "evadialog.h"
#include "ui_evadialog.h"
#include<QSqlRecord>
#include<QCheckBox>
#include<QDebug>


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



EvaDialog::EvaDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::EvaDialog)
{
    ui->setupUi(this);

    connect(ui->evaButton,SIGNAL(clicked(bool)),this,SLOT(evaluate()));


    ui->evaTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

//    QStringList header;
//    header.append("时间");
//    header.append("15秒实际");
//    header.append("15秒理论");
//    header.append("30秒实际");
//    header.append("30秒理论");
//    header.append("45秒实际");
//    header.append("45秒理论");
//    header.append("60秒实际");
//    header.append("60秒理论");
//    header.append("评价指数");

//    ui->tableWidget->setHorizontalHeaderLabels(header);

}

EvaDialog::~EvaDialog()
{
    delete ui;
}

void EvaDialog::initModel(QSqlTableModel *model)
{
    dataModel=model;
    initListWidget();
}

void EvaDialog::evaluate()
{
    //clear
    int temp=ui->evaTable->rowCount();

    for(int i=0;i<temp;i++)
    {
        ui->evaTable->removeRow(0);
    }

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


    //计算每个待评价机组的评价指数

    for(int i=0;i<eleNameList.size();i++)
    {
        double value=evaluate(eleNameList.at(i),d1,d2);

        ui->evaTable->insertRow(i);

        ui->evaTable->setItem(i,0,new QTableWidgetItem(eleNameList.at(i)));
        ui->evaTable->setItem(i,1,new QTableWidgetItem(QString::number(value)));
    }
}


double EvaDialog::evaluate(const QString &eleName,SimpleDate d1,SimpleDate d2)
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

            double tempValue=evaluate(r15,t15,r30,t30,r45,t45,r60,t60);

            result+=tempValue;

            count++;
        }
    }


    return count==0?0:(result/count);
}

void EvaDialog::initListWidget()
{
//    QListWidgetItem * item = new QListWidgetItem ();
//    QCheckBox * box = new QCheckBox();
//    box->setCheckable(true);
//    ui->listWidget->addItem(item);
//    ui->listWidget->setItemWidget(item,box);


    //clear
    int temp=ui->eleTable->rowCount();

    for(int i=0;i<temp;i++)
    {
        ui->eleTable->removeRow(0);
    }

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

double EvaDialog::evaluate(double r15, double t15, double r30, double t30, double r45, double t45, double r60, double t60)
{
    double k1=ui->k1SpinBox->value();
    double k2=ui->k2SpinBox->value();
    double k3=ui->k3SpinBox->value();
    double k4=ui->k4SpinBox->value();


    double value1=(t15==0?0:(r15/t15));
    double value2=(t30==0?0:(r30/t30));
    double value3=(t45==0?0:(r45/t45));
    double value4=(t60==0?0:(r60/t60));

    return k1*value1+k2*value2+k3*value3+k4*value4;
}
