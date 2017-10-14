#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include"ele.h"
#include"eleitem.h"
#include"csv.h"
#include"QTextStream"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->action_O,SIGNAL(triggered(bool)),this,SLOT(openFileSlot()));
    connect(ui->listView,SIGNAL(clicked(QModelIndex)),this,SLOT(calculate()));
    connect(ui->k1SpinBox,SIGNAL(valueChanged(double)),this,SLOT(calculate()));
    connect(ui->k2SpinBox,SIGNAL(valueChanged(double)),this,SLOT(calculate()));
    connect(ui->k3SpinBox,SIGNAL(valueChanged(double)),this,SLOT(calculate()));
    connect(ui->k4SpinBox,SIGNAL(valueChanged(double)),this,SLOT(calculate()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFileSlot()
{
    auto curFile=QFileDialog::getOpenFileName(this,
                                         tr("Open csv file"),".",
                                         tr("csv file(*.csv)"));
    open(curFile);
}

void MainWindow::calculate()
{
    if(!model)
    {
        return;
    }

    auto index=ui->listView->currentIndex();

    EleItem* item=static_cast<EleItem*>(model->itemFromIndex(index));

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
    model=new QStandardItemModel();

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
        model->appendRow(item);
    }
    ui->listView->setModel(model);
}
