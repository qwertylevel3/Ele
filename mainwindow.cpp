#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include"ele.h"
#include"eleitem.h"
#include"csv.h"
#include"QTextStream"
#include"importdialog.h"
#include"eledatasetitem.h"
#include"QMessageBox"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->action_O,SIGNAL(triggered(bool)),this,SLOT(importFileSlot()));
    connect(ui->dataSetList,SIGNAL(clicked(QModelIndex)),this,SLOT(openDataSlot()));
    connect(ui->listView,SIGNAL(clicked(QModelIndex)),this,SLOT(calculate()));
    connect(ui->k1SpinBox,SIGNAL(valueChanged(double)),this,SLOT(calculate()));
    connect(ui->k2SpinBox,SIGNAL(valueChanged(double)),this,SLOT(calculate()));
    connect(ui->k3SpinBox,SIGNAL(valueChanged(double)),this,SLOT(calculate()));
    connect(ui->k4SpinBox,SIGNAL(valueChanged(double)),this,SLOT(calculate()));


    eleDataSetModel=new QStandardItemModel();
    this->ui->dataSetList->setModel(eleDataSetModel);
    load();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openDataSlot()
{
    auto index=ui->dataSetList->currentIndex();

    EleDataSetItem* item=static_cast<EleDataSetItem*>(eleDataSetModel->itemFromIndex(index));

    QString filepath=item->getFilePath();
    open(filepath);

}

void MainWindow::importFileSlot()
{
    auto curFile=QFileDialog::getOpenFileName(this,
                                              tr("Open csv file"),".",
                                              tr("csv file(*.csv)"));
    ImportDialog dialog(this);
    if(dialog.exec())
    {
        QString dataName=dialog.getDataName();

        import(dataName,curFile);
        save();
    }

}

void MainWindow::calculate()
{
    if(!eleModel)
    {
        return;
    }

    auto index=ui->listView->currentIndex();

    EleItem* item=static_cast<EleItem*>(eleModel->itemFromIndex(index));

    Ele ele=item->getEleData();

    double k1=ui->k1SpinBox->value();
    double k2=ui->k2SpinBox->value();
    double k3=ui->k3SpinBox->value();
    double k4=ui->k4SpinBox->value();

    double e15=ele.getData(19);
    double e15The=ele.getData(20);
    double e30=ele.getData(22);
    double e30The=ele.getData(23);
    double e45=ele.getData(25);
    double e45The=ele.getData(26);
    double e60=ele.getData(28);
    double e60The=ele.getData(29);

    ui->lineEdit15->setText(QString::number(e15));
    ui->lineEdit30->setText(QString::number(e30));
    ui->lineEdit45->setText(QString::number(e45));
    ui->lineEdit60->setText(QString::number(e60));

    ui->lineEdit15The->setText(QString::number(e15The));
    ui->lineEdit30The->setText(QString::number(e30The));
    ui->lineEdit45The->setText(QString::number(e45The));
    ui->lineEdit60The->setText(QString::number(e60The));

    double temp1=(e15The==0?0:(e15/e15The));
    double temp2=(e30The==0?0:(e30/e30The));
    double temp3=(e45The==0?0:(e45/e45The));
    double temp4=(e60The==0?0:(e60/e60The));


    double value=k1*temp1+k2*temp2+k3*temp3+k4*temp4;
    ui->lineEdit->setText(QString::number(value));

}

void MainWindow::open(QString filename)
{
    if(eleModel)
    {
        eleModel->clear();
    }
    eleModel=new QStandardItemModel();

    QFile file(filename);
    QStringList csvList;
    if(file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);

        while(!stream.atEnd())
        {
            csvList.push_back(stream.readLine());
        }
        file.close();
    }

    for(int i=0;i<csvList.size();i++)
    {
        Ele ele;
        ele.init(csvList.at(i));
        EleItem* item=new EleItem();
        item->setEleData(ele);
        eleModel->appendRow(item);
    }
    ui->listView->setModel(eleModel);
}

void MainWindow::import(QString dataName, QString fileName)
{
    EleDataSetItem* dataSetItem=new EleDataSetItem();

    dataSetItem->setDataName(dataName);
    dataSetItem->setFilePath(fileName);

    eleDataSetModel->appendRow(dataSetItem);

    ui->dataSetList->setCurrentIndex(dataSetItem->index());
}

void MainWindow::save()
{
    QString dataSetPath="data.db";
    QFile file(dataSetPath);

    if(!file.open(QIODevice::WriteOnly  | QIODevice::Text))
    {
        QMessageBox::warning(this,"sdf","can't open",QMessageBox::Yes);
    }
    QTextStream in(&file);
    for(int i=0;i<eleDataSetModel->rowCount();i++)
    {
        EleDataSetItem* item=static_cast<EleDataSetItem*>(eleDataSetModel->item(i));
        in<<item->getDataName()<<"|"<<item->getFilePath()<<"\n";

    }
    file.close();
}

void MainWindow::load()
{
    QString dataSetPath="data.db";
    QFile file(dataSetPath);
    QStringList dataList;
    if(file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);

        while(!stream.atEnd())
        {
            dataList.push_back(stream.readLine());
        }
        file.close();
    }

    for(int i=0;i<dataList.size();i++)
    {
        QStringList infoList=dataList.at(i).split("|");
        import(infoList.at(0),infoList.at(1));
    }
}

