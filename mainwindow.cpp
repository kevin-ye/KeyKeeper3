#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    handlerInstance(dataBaseHandler::getInstance())
{
    ui->setupUi(this);
    ui->removeButton->setStyleSheet(QString("QPushButton {color: red; }"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
