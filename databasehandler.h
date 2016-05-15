#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <string>

// singleton database handler class
class dataBaseHandler
{
    bool loginFlag;

    dataBaseHandler();

public:
    static dataBaseHandler *getInstance();

    bool loginWithPassword(std::string password);
};

#endif // DATABASEHANDLER_H
