#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

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

private:
    Ui::MainWindow *ui;
    dataBaseHandler *handlerInstance;
    QStandardItemModel *theModel;
    void refreshTable();
};

#endif // MAINWINDOW_H
