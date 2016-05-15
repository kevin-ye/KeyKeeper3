#include "logindialog.h"
#include "ui_logindialog.h"

using namespace std;

loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog),
    handlerInstance(dataBaseHandler::getInstance())
{
    ui->setupUi(this);
    //ui->cancelButton->setStyleSheet(QString("QPushButton {color: red; }"));
}

void loginDialog::exitApplication()
{
    QApplication::quit();
}

loginDialog::~loginDialog()
{
    delete ui;
}

void loginDialog::on_loginButton_clicked()
{
    // login
    QString password = this->ui->passwordEdit->text();

    if (handlerInstance->loginWithPassword(password)) {
        // success
    } else {
        // error
        exitApplication();
    }
}

void loginDialog::on_cancelButton_clicked()
{
    exitApplication();
}
