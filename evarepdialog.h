#ifndef EVAREPDIALOG_H
#define EVAREPDIALOG_H

#include <QDialog>
#include<QSqlTableModel>
#include<QStandardItemModel>
#include"simpledate.h"

namespace Ui {
class EvaRepDialog;
}

class EvaRepDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EvaRepDialog(QWidget *parent = 0);
    ~EvaRepDialog();
    void setModel(QSqlTableModel* m);
public slots:
    void generateRep();

private:
    void initHeaderTable();
    void initListWidget();
    //d1->d2之间eleName评价指数
    double evaluate(const QString& eleName,SimpleDate d1,SimpleDate d2);
    //d1->d2之间eleName机组header项的均值
    double getTableValue(const QString& eleName,
                         const QString& header,
                         SimpleDate d1,SimpleDate d2
                         );

    void generateHeader(QStandardItemModel* model);
    Ui::EvaRepDialog *ui;
    QSqlTableModel* dataModel{nullptr};
};

#endif // EVAREPDIALOG_H
