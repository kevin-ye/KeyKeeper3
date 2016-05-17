#include "logindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    loginDialog w;
    w.setFixedSize(w.size());
    w.show();

    return a.exec();
}
