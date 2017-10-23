#include "evadialog.h"
#include "ui_evadialog.h"
#include<QSqlRecord>
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

//    connect(ui->k1SpinBox,SIGNAL(valueChanged(double)),this,SLOT(calculate()));
//    connect(ui->k2SpinBox,SIGNAL(valueChanged(double)),this,SLOT(calculate()));
//    connect(ui->k3SpinBox,SIGNAL(valueChanged(double)),this,SLOT(calculate()));
//    connect(ui->k4SpinBox,SIGNAL(valueChanged(double)),this,SLOT(calculate()));

    //    connect(ui->dateEdit1,SIGNAL(timeChanged(QTime)),this,SLOT(search()));
    //    connect(ui->dateEdit2,SIGNAL(timeChanged(QTime)),this,SLOT(search()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(search()));

    QStringList header;
    header.append("时间");
    header.append("15秒实际");
    header.append("15秒理论");
    header.append("30秒实际");
    header.append("30秒理论");
    header.append("45秒实际");
    header.append("45秒理论");
    header.append("60秒实际");
    header.append("60秒理论");

    ui->tableWidget->setHorizontalHeaderLabels(header);
}

EvaDialog::~EvaDialog()
{
    delete ui;
}

void EvaDialog::setModel(QSqlTableModel *model)
{
    dataModel=model;
}

void EvaDialog::setEleName(QString name)
{
    eleName=name;
}

void EvaDialog::search()
{
    int temp=ui->tableWidget->rowCount();

    for(int i=0;i<temp;i++)
    {
        ui->tableWidget->removeRow(0);
    }


    dataModel->select();

    QDate date1=ui->dateEdit1->date();
    QDate date2=ui->dateEdit2->date();

    SimpleDate d1;
    SimpleDate d2;

    d1.init(date1);
    d2.init(date2);


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
            QStandardItem* row=new QStandardItem();

            QString strR15=(dataModel->record(i)).value("15秒实际贡献电量").toString();
            QString strT15=(dataModel->record(i)).value("15秒理论贡献电量").toString();
            QString strR30=(dataModel->record(i)).value("30秒实际贡献电量").toString();
            QString strT30=(dataModel->record(i)).value("15秒理论贡献电量").toString();
            QString strR45=(dataModel->record(i)).value("45秒实际贡献电量").toString();
            QString strT45=(dataModel->record(i)).value("15秒理论贡献电量").toString();
            QString strR60=(dataModel->record(i)).value("60秒实际贡献电量").toString();
            QString strT60=(dataModel->record(i)).value("15秒理论贡献电量").toString();

            QTableWidgetItem* dateTimeItem=new QTableWidgetItem();
            dateTimeItem->setText(dateTime);

            QTableWidgetItem* r15=new QTableWidgetItem();
            r15->setText(strR15);
            QTableWidgetItem* t15=new QTableWidgetItem();
            t15->setText(strT15);
            QTableWidgetItem* r30=new QTableWidgetItem();
            r30->setText(strR30);
            QTableWidgetItem* t30=new QTableWidgetItem();
            t30->setText(strT30);
            QTableWidgetItem* r45=new QTableWidgetItem();
            r45->setText(strR45);
            QTableWidgetItem* t45=new QTableWidgetItem();
            t45->setText(strT45);
            QTableWidgetItem* r60=new QTableWidgetItem();
            r60->setText(strR60);
            QTableWidgetItem* t60=new QTableWidgetItem();
            t60->setText(strT60);

            ui->tableWidget->insertRow(count);

            ui->tableWidget->setItem(count,0,dateTimeItem);
            ui->tableWidget->setItem(count,1,r15);
            ui->tableWidget->setItem(count,2,t15);
            ui->tableWidget->setItem(count,3,r30);
            ui->tableWidget->setItem(count,4,t30);
            ui->tableWidget->setItem(count,5,r45);
            ui->tableWidget->setItem(count,6,t45);
            ui->tableWidget->setItem(count,7,r60);
            ui->tableWidget->setItem(count,8,t60);

            count++;

        }
    }
    calculate();
}

void EvaDialog::calculate()
{
    int sumCount=0;
    int goodCount=0;

    int temp=ui->tableWidget->rowCount();
    qDebug()<<temp;

    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        double r15=ui->tableWidget->item(i,1)->text().toDouble();
        double t15=ui->tableWidget->item(i,2)->text().toDouble();
        double r30=ui->tableWidget->item(i,3)->text().toDouble();
        double t30=ui->tableWidget->item(i,4)->text().toDouble();
        double r45=ui->tableWidget->item(i,5)->text().toDouble();
        double t45=ui->tableWidget->item(i,6)->text().toDouble();
        double r60=ui->tableWidget->item(i,7)->text().toDouble();
        double t60=ui->tableWidget->item(i,8)->text().toDouble();

        double value=evaluate(r15,t15,r30,t30,r45,t45,r60,t60);

        if(value>0.6)
        {
            goodCount++;
        }
        sumCount++;
    }

    double per=double(goodCount)/double(sumCount);

    ui->lineEdit1->setText(QString::number(sumCount));
    ui->lineEdit2->setText(QString::number(sumCount));
    ui->lineEdit3->setText(QString::number(per));
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
