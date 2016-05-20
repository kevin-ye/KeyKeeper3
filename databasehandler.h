#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QSqlDatabase>
#include <QString>
#include <vector>
#include <string>

#include <cryptopp/secblock.h>

// singleton database handler class
class dataBaseHandler
{
    bool loginFlag;
    CryptoPP::SecByteBlock *storedKey;
    QSqlDatabase _db;

    dataBaseHandler();

    void expireKey();
    void passwordToKey(const QString &password);
    void initRand();
    int getRand();
    QString generateSalt();
    QString hashPassword(const QString &password, const QString &salt);
    std::string decrypt(const std::string &ciphertext);
    std::string encrypt(const std::string &plain);

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
        std::string title;
        std::string username;
        std::string password;
        std::string note;
    };

    /***
     * fetch userData by index
     ***/
     void getmodelData(std::vector<dataBaseHandler::modelData> &modify, const unsigned int index = 0, const bool fetchAll = true);
};

#endif // DATABASEHANDLER_H
