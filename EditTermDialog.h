#ifndef EDITTERMDIALOG_H
#define EDITTERMDIALOG_H

#include <QDialog>

namespace Ui {
class AddTermDialog;
}

class EditTermDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EditTermDialog(int termId, QString termName, QWidget *parent = 0);
    ~EditTermDialog();

    QString termName();

public slots:
    virtual void accept();

private:
    Ui::AddTermDialog *ui;

    int _termId;
    QString _termName;

};

#endif // EDITTERMDIALOG_H
