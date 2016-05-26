#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addnewdialog.h"

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
        theModel->setData(index, QString::fromStdString(item.title));
        index = theModel->index(i, 1);
        theModel->setData(index, QString::fromStdString(item.username));
        index = theModel->index(i, 2);
        theModel->setData(index, QString::fromStdString(item.note));

        ++i;
    }

    ui->tableView->repaint();
}

void MainWindow::on_refreshButton_clicked()
{
    refreshTable();
}

void MainWindow::on_addButton_clicked()
{
    // add new
    // create new "add new" dialog
    addNewDialog *w = new addNewDialog();
    // show dialog
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
    // refresh table
    refreshTable();
}
