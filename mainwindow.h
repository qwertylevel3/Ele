#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void openDataSlot();
    void importFileSlot();
    void calculate();
private:
    void open(QString filename);
    void import(QString dataName,QString fileName);
    void save();
    void load();
    Ui::MainWindow *ui;
    QStandardItemModel* eleModel{nullptr};
    QStandardItemModel* eleDataSetModel{nullptr};

};

#endif // MAINWINDOW_H
