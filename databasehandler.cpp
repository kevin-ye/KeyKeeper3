#include "databasehandler.h"

#include <QString>
#include <cstring>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QCryptographicHash>
#include <QByteArray>

#include <ctime>
#include <cstring>

#include "common.h"

using namespace std;

dataBaseHandler::dataBaseHandler() :
    loginFlag(false),
    storedPassword(nullptr)
{
    _db = QSqlDatabase::addDatabase(kkCommon::dbType);
    _db.setDatabaseName(kkCommon::dbPath);
}

void dataBaseHandler::expirePassword()
{
    if (storedPassword == nullptr) {
        return;
    }

    for (int i = 0; i < strlen(storedPassword); ++i)
    {
        char rc = getRand() % 128;
        // prevent compiler optimization
        ((char volatile *)storedPassword)[i] = rc;
    }

    delete [] storedPassword;
}

void dataBaseHandler::initRand()
{
    srand(time(NULL));
}

int dataBaseHandler::getRand()
{
    return rand();
}

dataBaseHandler *dataBaseHandler::getInstance()
{
    static dataBaseHandler _instance;

    return &_instance;
}

bool dataBaseHandler::loginWithPassword(QString &password)
{
    if ((password.length() == 0) || (password.length() > 255))  {
        return false;
    }

    loginFlag = _db.open();
    if (loginFlag) {
        try
        {
            expirePassword();
            storedPassword = new char[password.length() + 1];
            strcpy(storedPassword, password.toStdString().c_str());
            // fetch user salt
            QSqlQuery query;
            query.clear();
            query.prepare("select salt from validation");
            loginFlag = loginFlag && query.exec() && (query.record().count() == 1);
            QString salt = query.value(query.record().indexOf("salt")).toString();
            // hash with salt
            QByteArray hashedValue = QCryptographicHash::hash((password + salt).toLocal8Bit(), QCryptographicHash::Sha3_512);
            // matching hash
            query.clear();
            query.prepare("select * from validation where hash = (:hashedvalue)");
            query.bindValue(":hashedvalue", hashedValue);
            loginFlag = loginFlag && query.exec() && (query.record().count() == 1);
        } catch (...) {
            expirePassword();
            loginFlag = false;
        }
    }

    return loginFlag;
}
