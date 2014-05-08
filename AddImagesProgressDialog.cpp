#include "AddImagesProgressDialog.h"
#include "ui_AddImagesProgressDialog.h"
#include "AddImagesThread.h"

AddImagesProgressDialog::AddImagesProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddImagesProgressDialog)
{
    ui->setupUi(this);
}

AddImagesProgressDialog::~AddImagesProgressDialog()
{
    delete ui;
}

int AddImagesProgressDialog::exec()
{

    return QDialog::exec();
}
