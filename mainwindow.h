#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <vector>

#include "databasehandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_refreshButton_clicked();

    void on_addButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_detailButton_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    dataBaseHandler *handlerInstance;
    QStandardItemModel *theModel;
    std::vector<dataBaseHandler::modelData> tableData;
    void refreshTable();
};

#endif // MAINWINDOW_H
