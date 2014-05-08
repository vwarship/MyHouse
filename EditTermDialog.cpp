#include "EditTermDialog.h"
#include "ui_AddTermDialog.h"
#include "TableTermsUtil.h"

EditTermDialog::EditTermDialog(int termId, QString termName, QWidget *parent) :
    QDialog(parent), _termId(termId), _termName(termName),
    ui(new Ui::AddTermDialog)
{
    ui->setupUi(this);

    setWindowTitle(tr("修改"));
    ui->termNameEdit->setText(termName);
    ui->termNameEdit->setSelection(0, termName.length());
}

EditTermDialog::~EditTermDialog()
{
    delete ui;
}

QString EditTermDialog::termName()
{
    return _termName;
}

void EditTermDialog::accept()
{
    _termName = ui->termNameEdit->text();

    if (_termName.isEmpty())
    {
        ui->termNameEdit->setFocus();
        return;
    }

    TableTermsUtil::updateName(_termId, _termName);

    QDialog::accept();
}
