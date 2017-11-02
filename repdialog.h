#ifndef REPDIALOG_H
#define REPDIALOG_H

#include <QDialog>
#include<QStandardItemModel>

namespace Ui {
class RepDialog;
}

class RepDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RepDialog(QWidget *parent = 0);
    ~RepDialog();

    void setModel(QStandardItemModel* m);
private:
    Ui::RepDialog *ui;
    QStandardItemModel* model;
};

#endif // REPDIALOG_H
