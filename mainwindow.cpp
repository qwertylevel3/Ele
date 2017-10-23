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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->action_O,SIGNAL(triggered(bool)),this,SLOT(importFileSlot()));
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(calculate()));


    eleModel=new QStandardItemModel();

    ui->listView->setModel(eleModel);

    load();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openDataSlot()
{
}

void MainWindow::importFileSlot()
{
}

void MainWindow::calculate()
{
    dataModel->select();

    auto index=ui->listView->currentIndex();
    auto item=eleModel->itemFromIndex(index);
    auto curEleName=item->text();

    EvaDialog dialog;
    dialog.setModel(dataModel);
    dialog.setEleName(curEleName);

    dialog.exec();
}

void MainWindow::open(QString filename)
{

}

void MainWindow::import(QString dataName, QString fileName)
{
}

void MainWindow::save()
{
}

void MainWindow::load()
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("ele.db");

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

    auto iter=tag.begin();
    while(iter!=tag.end())
    {
        QStandardItem* item=new QStandardItem;
        item->setText(*iter);
        eleModel->appendRow(item);

        iter++;
    }
}

