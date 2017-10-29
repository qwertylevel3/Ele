#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QStandardItemModel>
#include<QSqlTableModel>

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
    void calculate();

    void importSlot();
    void quitSlot();

    void newDBSlot();
    void openDBSlot();
    void closeDBSlot();

    void evaSlot();
private:
    void initDB(const QString& dbname);
    void initTableWidget();
    void load(const QString& dbname);
    Ui::MainWindow *ui;
    QSqlTableModel* dataModel{nullptr};

    QString curEleName;
};

#endif // MAINWINDOW_H
