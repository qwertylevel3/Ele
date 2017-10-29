#ifndef EVADIALOG_H
#define EVADIALOG_H

#include <QDialog>
#include<QSqlTableModel>
#include<QStandardItemModel>

namespace Ui {
class EvaDialog;
}



struct SimpleDate
{
    void init(const QString& s);
    void init(const QDate& date);

    int y;
    int m;
    int d;



    bool inRange(SimpleDate d1,SimpleDate d2);

    bool lessEqual(SimpleDate d);
};

class EvaDialog : public QDialog
{
    Q_OBJECT
public:


    explicit EvaDialog(QWidget *parent = 0);
    ~EvaDialog();

    void initModel(QSqlTableModel* model);
public slots:
    void evaluate();
private:
    double evaluate(const QString& eleName,SimpleDate d1,SimpleDate d2);
    void initListWidget();
    //获取某一天的评价指数
    double evaluate(double r15,double t15,
                    double r30,double t30,
                    double r45,double t45,
                    double r60,double t60);

    Ui::EvaDialog *ui;

    QSqlTableModel* dataModel{nullptr};

    QString eleName;
};

#endif // EVADIALOG_H
