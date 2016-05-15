#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

#include "databasehandler.h"

namespace Ui {
class loginDialog;
}

class loginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit loginDialog(QWidget *parent = 0);
    ~loginDialog();

private slots:
    void on_loginButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::loginDialog *ui;
    dataBaseHandler *handlerInstance;

    void exitApplication();
};

#endif // LOGINDIALOG_H
