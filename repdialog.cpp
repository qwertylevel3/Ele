#include "repdialog.h"
#include "ui_repdialog.h"

RepDialog::RepDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RepDialog)
{
    ui->setupUi(this);
}

RepDialog::~RepDialog()
{
    delete ui;
}

void RepDialog::setModel(QStandardItemModel *m)
{
    model=m;
    ui->tableView->setModel(model);
}
