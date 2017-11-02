#ifndef ATRDIALOG_H
#define ATRDIALOG_H

#include <QDialog>
#include<QSqlTableModel>
#include<QStandardItemModel>
#include"simpledate.h"


namespace Ui {
class AtrDialog;
}

class AtrDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AtrDialog(QWidget *parent = 0);
    ~AtrDialog();

    void initModel(QSqlTableModel* model);
public slots:
    void evaluate();
private:
    double evaluate(const QString& eleName,SimpleDate d1,SimpleDate d2);
    void initListWidget();

    QSqlTableModel* dataModel{nullptr};

    Ui::AtrDialog *ui;
};

#endif // ATRDIALOG_H
