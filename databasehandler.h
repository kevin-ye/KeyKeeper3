#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QSqlDatabase>
#include <QString>

// singleton database handler class
class dataBaseHandler
{
    bool loginFlag;
    char *storedPassword;
    QSqlDatabase _db;

    dataBaseHandler();

    void expirePassword();
    void initRand();
    int getRand();

public:
    static dataBaseHandler *getInstance();

    bool loginWithPassword(QString &password);
};

#endif // DATABASEHANDLER_H
