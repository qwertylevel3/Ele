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
    void openFileSlot();
    void calculate();
private:
    void open(QString filename);
    Ui::MainWindow *ui;
    QStandardItemModel* model{nullptr};
};

#endif // MAINWINDOW_H
