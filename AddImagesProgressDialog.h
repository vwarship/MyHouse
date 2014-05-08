#ifndef ADDIMAGESPROGRESSDIALOG_H
#define ADDIMAGESPROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class AddImagesProgressDialog;
}

class AddImagesProgressDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddImagesProgressDialog(QWidget *parent = 0);
    ~AddImagesProgressDialog();
    
public Q_SLOTS:
    int exec();

private:
    Ui::AddImagesProgressDialog *ui;
};

#endif // ADDIMAGESPROGRESSDIALOG_H
