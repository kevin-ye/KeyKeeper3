#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QModelIndex>

#include <vector>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    handlerInstance(dataBaseHandler::getInstance()),
    theModel(nullptr)
{
    ui->setupUi(this);
    ui->removeButton->setStyleSheet(QString("QPushButton {color: red; }"));

    refreshTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshTable()
{
    vector<dataBaseHandler::modelData> tableData;
    tableData.clear();

    handlerInstance->getmodelData(tableData);
    theModel = new QStandardItemModel(tableData.size(), 3, this);
    theModel->setHorizontalHeaderLabels({"Title", "Username", "Note"});
    ui->tableView->setModel(theModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    unsigned int i = 0;
    for (auto &item : tableData) {
        QModelIndex index = theModel->index(i, 0);
        theModel->setData(index, item.title);
        index = theModel->index(i, 1);
        theModel->setData(index, item.username);
        index = theModel->index(i, 2);
        theModel->setData(index, item.note);

        ++i;
    }

    ui->tableView->repaint();
}

void MainWindow::on_refreshButton_clicked()
{
    refreshTable();
}
