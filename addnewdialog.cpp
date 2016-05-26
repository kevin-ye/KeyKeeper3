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

void addNewDialog::accepted()
{
    qDebug() << "accept" << endl;
}

void addNewDialog::on_buttonBox_accepted()
{
    qDebug() << "save" << endl;
    // save
    if (isModifing) {
        // save to index
    } else {
        // insert new
    }
}

void addNewDialog::on_buttonBox_rejected()
{
    qDebug() << "cancel" << endl;
    // cancel
    this->reject();
}
