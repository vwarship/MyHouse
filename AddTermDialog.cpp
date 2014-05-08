#include "AddTermDialog.h"
#include "ui_AddTermDialog.h"
#include "Sqlite.h"

AddTermDialog::AddTermDialog(Sqlite *sqlite, int parentId, QString taxonomy, QWidget *parent) :
    QDialog(parent), _sqlite(sqlite), _parentId(parentId), _taxonomy(taxonomy),
    ui(new Ui::AddTermDialog)
{
    ui->setupUi(this);
}

AddTermDialog::~AddTermDialog()
{
    delete ui;
}

QString AddTermDialog::termName()
{
    return ui->termNameEdit->text();
}

void AddTermDialog::accept()
{
    QString name = termName();
    if (name.isEmpty())
    {
        ui->termNameEdit->setFocus();
        return;
    }

    _sqlite->insertTermTaxonomy(name, _parentId, _taxonomy);

    QDialog::accept();
}
