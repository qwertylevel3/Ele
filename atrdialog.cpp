#include "atrdialog.h"
#include "ui_atrdialog.h"
#include"util.h"
#include<QSqlRecord>
#include<QCheckBox>


AtrDialog::AtrDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::AtrDialog)
{
    ui->setupUi(this);

    connect(ui->evaButton,SIGNAL(clicked(bool)),this,SLOT(evaluate()));


    ui->evaTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

AtrDialog::~AtrDialog()
{
    delete ui;
}

void AtrDialog::initModel(QSqlTableModel *model)
{
    dataModel=model;
    initListWidget();
}

void AtrDialog::evaluate()
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

double AtrDialog::evaluate(const QString &eleName, SimpleDate d1, SimpleDate d2)
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

        double freDiff=-(dataModel->record(i)).value("频差极值").toDouble();

        double freThreshold=ui->doubleSpinBox->value();

        if(d.inRange(d1,d2) && freDiff>freThreshold)
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

void AtrDialog::initListWidget()
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
