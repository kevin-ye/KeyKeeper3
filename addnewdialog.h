#ifndef ADDNEWDIALOG_H
#define ADDNEWDIALOG_H

#include <QDialog>

#include "databasehandler.h"

namespace Ui {
class addNewDialog;
}

class addNewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addNewDialog(QWidget *parent = 0, bool isModifing = false, const dataBaseHandler::modelData &theData = {0, "", "", "", ""});
    ~addNewDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::addNewDialog *ui;
    dataBaseHandler *handlerInstance;
    dataBaseHandler::modelData holdingData;
    const bool isModifing;

    void accepted();
};

#endif // ADDNEWDIALOG_H
