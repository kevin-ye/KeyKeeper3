#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QSqlDatabase>
#include <QString>
#include <vector>

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
    QString hashPassword(const QString &password, const QString &salt);
    QString decrypt(const QString &cipher);
    QString encrypt(const QString &plainText);

public:
    ~dataBaseHandler();

    static dataBaseHandler *getInstance();

    /***
     * create a new database
     * with given password
     * return true on success
     ***/
    bool createNewDatabase(const QString &password);

    /***
     * login the database
     * with given password
     * return true on success
     ***/
    bool loginWithPassword(const QString &password);

    // return true if there is no database
    bool needToCreateDatabase();

    // return true if the login is complete
    bool isReady();

    // modelData struct
    struct modelData {
        unsigned int index;
        QString title;
        QString username;
        QString note;
    };

    /***
     * fetch userData by index
     ***/
     void getmodelData(std::vector<dataBaseHandler::modelData> &modify, const unsigned int index = 0, const bool fetchAll = true);
};

#endif // DATABASEHANDLER_H
