#include "databasehandler.h"

#include <QString>

#include "common.h"


using namespace std;

dataBaseHandler::dataBaseHandler() :
    loginFlag(false)
{
    _db = QSqlDatabase::addDatabase(kkCommon::dbType);
    _db.setDatabaseName(kkCommon::dbPath);
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

    loginFlag = _db.open(kkCommon::dbUser, password);

    return loginFlag;
}
