#include "logindialog.h"
#include "ui_logindialog.h"

using namespace std;

loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog),
    handlerInstance(dataBaseHandler::getInstance())
{
    ui->setupUi(this);
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
    string password = this->ui->passwordEdit->text().toStdString();

    if (handlerInstance->loginWithPassword(password)) {
        // success
    } else {
        // error
        exitApplication();
    }
}
