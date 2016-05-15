#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QSqlDatabase>
#include <QString>

// singleton database handler class
class dataBaseHandler
{
    bool loginFlag;
    QSqlDatabase _db;

    dataBaseHandler();

public:
    static dataBaseHandler *getInstance();

    bool loginWithPassword(QString &password);
};

#endif // DATABASEHANDLER_H
