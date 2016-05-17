#ifndef COMMON
#define COMMON

#endif // COMMON

#include <QString>

namespace kkCommon {
    const QString dbType = "QSQLITE";
    const QString dbFolder = "database";
    const QString dbPath = "database/data.db";
    const QString dbuserDataSchema =
            "CREATE TABLE \'userData\' (\n"
            "\'id\' INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,\n"
            "\'title\' TEXT NOT NULL,\n"
            "\'username\' TEXT DEFAULT \'no name\',\n"
            "\'password\' TEXT DEFAULT \'no password\',\n"
            "\'note\' TEXT\n"
            ");";
    const QString dbvalidationSchema =
            "CREATE TABLE \"validation\" (\n"
            "\'hash\' TEXT NOT NULL,\n"
            "\'salt\' TEXT NOT NULL\n"
            ")";
    const int saltLength = 128;
}
