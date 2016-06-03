#include "addnewdialog.h"
#include "ui_addnewdialog.h"

#include <QDebug>

addNewDialog::addNewDialog(QWidget *parent, bool isModifing, const dataBaseHandler::modelData &theData) :
    QDialog(parent),
    ui(new Ui::addNewDialog),
    holdingData(theData),
    isModifing(isModifing),
    handlerInstance(dataBaseHandler::getInstance())
{
    ui->setupUi(this);
    if (isModifing) {
        // display data
        ui->titleEdit->setText(QString::fromStdString(holdingData.title));
        ui->usernameEdit->setText(QString::fromStdString(holdingData.username));
        ui->passwordEdit->setText(QString::fromStdString(holdingData.password));
        ui->noteEdit->setText(QString::fromStdString(holdingData.note));
    }
}

addNewDialog::~addNewDialog()
{
    delete ui;
}

void addNewDialog::on_buttonBox_accepted()
{
    holdingData.title = ui->titleEdit->text().toStdString();
    holdingData.username = ui->usernameEdit->text().toStdString();
    holdingData.password = ui->usernameEdit->text().toStdString();
    holdingData.note = ui->noteEdit->toPlainText().toStdString();
    if (isModifing) {
        // save to index
        handlerInstance->saveOnIndex(holdingData);
    } else {
        // insert new
        handlerInstance->insertNew(holdingData);
    }
    this->accept();
}

void addNewDialog::on_buttonBox_rejected()
{
    // cancel
    this->reject();
}
