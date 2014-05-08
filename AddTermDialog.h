#ifndef ADDTERMDIALOG_H
#define ADDTERMDIALOG_H

#include <QDialog>

namespace Ui {
class AddTermDialog;
}

class Sqlite;

class AddTermDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddTermDialog(Sqlite *sqlite, int parentId, QString taxonomy, QWidget *parent = 0);
    ~AddTermDialog();

    QString termName();
    
public slots:
    virtual void accept();

private:
    Ui::AddTermDialog *ui;
    Sqlite *_sqlite;
    int _parentId;
    QString _taxonomy;

};

#endif // ADDTERMDIALOG_H
