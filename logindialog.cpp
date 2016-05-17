#include "logindialog.h"
#include "ui_logindialog.h"

#include <QMessageBox>

#include "mainwindow.h"

using namespace std;

loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog),
    handlerInstance(dataBaseHandler::getInstance())
{
    ui->setupUi(this);

    if (!handlerInstance->needToCreateDatabase()) {
        // no need to create a new database
        ui->confirmEdit->setVisible(false);
        ui->confirmLabel->setVisible(false);
        ui->loginButton->setText("Login");
    } else {
        ui->confirmEdit->setVisible(true);
        ui->confirmLabel->setVisible(true);
        ui->loginButton->setText("Create");
    }
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
    if (ui->loginButton->text() == "Login") {
        // login
        QString password = this->ui->passwordEdit->text();

        if (handlerInstance->loginWithPassword(password)) {
            // success
            if (handlerInstance->isReady()) {
                MainWindow *theMainWindow = new MainWindow();
                theMainWindow->setAttribute(Qt::WA_DeleteOnClose);
                this->hide();
                theMainWindow->show();
            }
        }
    } else if (ui->loginButton->text() == "Create") {
        QString password = this->ui->passwordEdit->text();
        QString confirm = this->ui->confirmEdit->text();
        if (password == confirm) {
            handlerInstance->createNewDatabase(password);
        }
    }

//    QMessageBox msgBox;
//    msgBox.setText("KeyKeeper");
//    if (handlerInstance->isReady()) {
//        msgBox.setInformativeText("ready");
//    } else {
//        msgBox.setInformativeText("error");
//    }
//    msgBox.exec();
}

void loginDialog::on_cancelButton_clicked()
{
    exitApplication();
}
