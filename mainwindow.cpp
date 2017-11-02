#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include"ele.h"
#include"eleitem.h"
#include"csv.h"
#include"QTextStream"
#include"importdialog.h"
#include<QSqlDatabase>
#include"QMessageBox"
#include<QSqlError>
#include<QDebug>
#include<QSqlRecord>
#include<evadialog.h>
#include"atrdialog.h"
#include"evarepdialog.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionImport,SIGNAL(triggered(bool)),this,SLOT(importSlot()));
    connect(ui->actionQuit,SIGNAL(triggered(bool)),SLOT(quitSlot()));

    connect(ui->actionNew,SIGNAL(triggered(bool)),this,SLOT(newDBSlot()));
    connect(ui->actionOpen,SIGNAL(triggered(bool)),this,SLOT(openDBSlot()));
    connect(ui->actionClose,SIGNAL(triggered(bool)),this,SLOT(closeDBSlot()));

    connect(ui->actionEva,SIGNAL(triggered(bool)),this,SLOT(evaSlot()));
    connect(ui->actionAtr,SIGNAL(triggered(bool)),this,SLOT(atrSlot()));
    connect(ui->actionEvaRep,SIGNAL(triggered(bool)),this,SLOT(evaRepSlot()));


   ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::calculate()
{
}

void MainWindow::importSlot()
{

}

void MainWindow::quitSlot()
{

}

void MainWindow::newDBSlot()
{

}

void MainWindow::openDBSlot()
{
    auto curFile=QFileDialog::getOpenFileName(this,
                                              tr("Open db file"),".",
                                              tr("db file(*.db)"));

    load(curFile);
    ui->actionImport->setEnabled(true);
    ui->actionClose->setEnabled(true);
    ui->actionEva->setEnabled(true);
    ui->actionAtr->setEnabled(true);
    ui->actionEvaRep->setEnabled(true);
}

void MainWindow::closeDBSlot()
{
    delete dataModel;
    dataModel=nullptr;

    ui->tableWidget->clearContents();

    ui->actionImport->setEnabled(false);
    ui->actionClose->setEnabled(false);
    ui->actionEva->setEnabled(false);
    ui->actionAtr->setEnabled(false);
    ui->actionEvaRep->setEnabled(false);

    QMessageBox msgBox;
    msgBox.setText("数据库已关闭");
    msgBox.exec();
}

void MainWindow::evaSlot()
{
    dataModel->select();

    EvaDialog dialog;
    dialog.initModel(dataModel);

    dialog.exec();
}

void MainWindow::atrSlot()
{
    dataModel->select();

    AtrDialog dialog;
    dialog.initModel(dataModel);

    dialog.exec();
}

void MainWindow::evaRepSlot()
{
   EvaRepDialog dialog;
   dialog.setModel(dataModel);
   dialog.exec();
}

void MainWindow::initDB(const QString &dbname)
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(dbname);

    if(!db.open())
    {
        qDebug()<<"Error : can not open the ele db"<<db.lastError();
        return;
    }

    dataModel=new QSqlTableModel(this,db);
    dataModel->setTable("test");
    dataModel->select();

    //    qDebug()<<dataModel->rowCount();

    QSet<QString> tag;

    for(int i=0;i<dataModel->rowCount();i++)
    {
        QString name=(dataModel->record(i)).value("机组名称").toString();
        if(!tag.contains(name))
        {
            tag.insert(name);
        }
    }

    //    auto iter=tag.begin();
    //    while(iter!=tag.end())
    //    {
    //        QStandardItem* item=new QStandardItem;
    //        item->setText(*iter);
    //        eleModel->appendRow(item);
    //
    //        iter++;
    //    }


}

void MainWindow::initTableWidget()
{
    int temp=ui->tableWidget->rowCount();

    for(int i=0;i<temp;i++)
    {
        ui->tableWidget->removeRow(0);
    }

    dataModel->select();

    for(int i=0;i<dataModel->rowCount();i++)
    {
        QString name=(dataModel->record(i)).value("机组名称").toString();

        QString dateTime=(dataModel->record(i)).value("响应时间").toString();

        QString strR15=(dataModel->record(i)).value("15秒实际贡献电量").toString();
        QString strT15=(dataModel->record(i)).value("15秒理论贡献电量").toString();
        QString strR30=(dataModel->record(i)).value("30秒实际贡献电量").toString();
        QString strT30=(dataModel->record(i)).value("15秒理论贡献电量").toString();
        QString strR45=(dataModel->record(i)).value("45秒实际贡献电量").toString();
        QString strT45=(dataModel->record(i)).value("15秒理论贡献电量").toString();
        QString strR60=(dataModel->record(i)).value("60秒实际贡献电量").toString();
        QString strT60=(dataModel->record(i)).value("15秒理论贡献电量").toString();

        QTableWidgetItem* eleNameItem=new QTableWidgetItem();
        eleNameItem->setText(name);

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

        ui->tableWidget->insertRow(i);

        ui->tableWidget->setItem(i,0,eleNameItem);
        ui->tableWidget->setItem(i,1,dateTimeItem);
        ui->tableWidget->setItem(i,2,r15);
        ui->tableWidget->setItem(i,3,t15);
        ui->tableWidget->setItem(i,4,r30);
        ui->tableWidget->setItem(i,5,t30);
        ui->tableWidget->setItem(i,6,r45);
        ui->tableWidget->setItem(i,7,t45);
        ui->tableWidget->setItem(i,8,r60);
        ui->tableWidget->setItem(i,9,t60);
    }
}

void MainWindow::load(const QString &dbname)
{
    initDB(dbname);
//    initTableWidget();
    QMessageBox msgBox;
    msgBox.setText("数据库已打开");
    msgBox.exec();
}

