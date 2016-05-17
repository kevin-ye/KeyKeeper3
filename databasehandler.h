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
    QString generateSalt();
    QString hashPassword(QString &password, QString &salt);

public:
    ~dataBaseHandler();

    static dataBaseHandler *getInstance();

    /***
     * create a new database
     * with given password
     * return true on success
     ***/
    bool createNewDatabase(QString &password);

    /***
     * login the database
     * with given password
     * return true on success
     ***/
    bool loginWithPassword(QString &password);

    // return true if there is no database
    bool needToCreateDatabase();

    // return true if the login is complete
    bool isReady();
};

#endif // DATABASEHANDLER_H
