#include "databasehandler.h"

using namespace std;

dataBaseHandler::dataBaseHandler() :
    loginFlag(false)
{
}

dataBaseHandler *dataBaseHandler::getInstance()
{
    static dataBaseHandler _instance;

    return &_instance;
}

bool dataBaseHandler::loginWithPassword(string password)
{
    if ((password.length() == 0) || (password.length() > 255))  {
        return false;
    }

    bool retFlag = false;

    // decrypt db
    //

    return retFlag;
}
