#include "databasehandler.h"

#include <QString>
#include <cstring>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QCryptographicHash>
#include <QByteArray>
#include <QFileInfo>
#include <QFile>
#include <cmath>
#include <QDir>
#include <QVariant>

#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <cryptopp/md5.h>

//#include <QDebug>
//#include <QSqlError>
//#include <QApplication>

#include <ctime>
#include <cstring>

#include "common.h"

using namespace std;
using namespace CryptoPP;

dataBaseHandler::dataBaseHandler() :
    loginFlag(false),
    storedPassword(nullptr)
{
    _db = QSqlDatabase::addDatabase(kkCommon::dbType);
    _db.setDatabaseName(kkCommon::dbPath);
}

dataBaseHandler::~dataBaseHandler()
{
    expirePassword();
}

void dataBaseHandler::expirePassword()
{
    if (storedPassword == nullptr) {
        return;
    }

    for (size_t i = 0; i < strlen(storedPassword); ++i)
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

QString dataBaseHandler::generateSalt()
{
    QString salt = "";
    for (int i = 0; i < kkCommon::saltLength; ++i)
    {
        QString rc = QString::fromStdString(std::to_string(getRand() % 128));
        salt += rc;
    }

    return salt;
}

QString dataBaseHandler::hashPassword(const QString &password, const QString &salt)
{
    QByteArray hashedValue = QCryptographicHash::hash((password + salt).toLocal8Bit(), QCryptographicHash::Sha3_512);
    QString hashedPassword = "";

    for (int i = 0; i < hashedValue.count(); ++i)
    {
        hashedPassword += QString::fromStdString(std::to_string(abs((int)(hashedValue.at(i)))));
    }

    //qDebug() << hashedPassword << endl;

    return hashedPassword;
}

QString dataBaseHandler::decrypt(const QString &cipher)
{
    string plain;
    string encrypted = storedPassword;
    // Hex decode symmetric key:
    HexDecoder decoder;
    decoder.Put((byte *)PRIVATE_KEY,32*2);
    decoder.MessageEnd();
    word64 size = decoder.MaxRetrievable();
    char *decodedKey = new char[size];
    decoder.Get((byte *)decodedKey, size);
    // Generate Cipher, Key, and CBC
    byte key[AES::MAX_KEYLENGTH], iv[AES::BLOCKSIZE];
    StringSource(reinterpret_cast<const char *>(decodedKey), true,
                 new HashFilter(*(new SHA256), new ArraySink(key, AES::MAX_KEYLENGTH)));
    memset(iv, 0x00, AES::BLOCKSIZE);
    try {
        CBC_Mode<AES>::Decryption Decryptor(key, sizeof(key), iv);
        StringSource(encrypted, true,
                     new HexDecoder(new StreamTransformationFilter(Decryptor,
                                                                   new StringSink(plain))));
    }
    catch (...) {
        // ...
    }

    return QString::fromStdString(plain);
}

QString dataBaseHandler::encrypt(const QString &plainText)
{

}

dataBaseHandler *dataBaseHandler::getInstance()
{
    static dataBaseHandler _instance;

    return &_instance;
}

bool dataBaseHandler::needToCreateDatabase()
{
    QFileInfo dbfile(kkCommon::dbPath);

    return !(dbfile.exists() && dbfile.isFile());
}

bool dataBaseHandler::createNewDatabase(const QString &password)
{
    if (!(needToCreateDatabase()) || (password.length() == 0) || (password.length() > 255))  {
        return false;
    }

    if (!QDir(kkCommon::dbFolder).exists()) {
        QDir().mkdir(kkCommon::dbFolder);
    }

    loginFlag = _db.open();
    if (loginFlag) {
        try
        {
            QSqlQuery query;
            query.clear();
            // create validation table
            query.exec(kkCommon::dbvalidationSchema);
            query.clear();
            // create userData table
            query.exec(kkCommon::dbuserDataSchema);
            // generate salt
            QString salt = generateSalt();
            // hash password with salt
            QString hashedValue = hashPassword(password, salt);
            // store hash and salt in validation table
            query.clear();
            query.prepare("insert into validation (hash, salt) values (:hash, :salt)");
            query.bindValue(":hash", hashedValue);
            query.bindValue(":salt", salt);
            loginFlag = query.exec();
            _db.commit();
        } catch (...) {
            loginFlag = false;
        }
    }

    loginFlag = loginFlag && (loginWithPassword(password));

    return loginFlag;
}

bool dataBaseHandler::loginWithPassword(const QString &password)
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
            query.first();
            QString salt = query.value(query.record().indexOf("salt")).toString();
            // hash with salt
            QString hashedValue = hashPassword(password, salt);
            // matching hash
            query.clear();
            query.prepare("select * from validation where hash = :hashedvalue");
            query.bindValue(":hashedvalue", hashedValue);
            loginFlag = loginFlag && query.exec() && (query.size() != 0);
        } catch (...) {
            expirePassword();
            loginFlag = false;
        }
    }

    return loginFlag;
}

bool dataBaseHandler::isReady()
{
    return loginFlag;
}

void dataBaseHandler::getmodelData(vector<dataBaseHandler::modelData> &modify, const unsigned int index, const bool fetchAll)
{
    if (!isReady()) {
        return;
    }

    modify.clear();
    try {
        if (fetchAll) {
            QSqlQuery query;
            query.clear();
            query.prepare("select id, title, username, note from userData");
            if (query.exec()) {
                while (query.next()) {
                    dataBaseHandler::modelData newRecord;
                    newRecord.index = query.value(query.record().indexOf("id")).toInt();
                    newRecord.title = query.value(query.record().indexOf("title")).toString();
                    newRecord.username = query.value(query.record().indexOf("username")).toString();
                    newRecord.note = query.value(query.record().indexOf("note")).toString();

                    modify.push_back(newRecord);
                }
            }
        } else {
            QSqlQuery query;
            query.clear();
            query.prepare("select id, title, username, note from userData where id = :index");
            query.bindValue(":index", index);
            if (query.exec()) {
                while (query.next()) {
                    dataBaseHandler::modelData newRecord;
                    newRecord.index = query.value(query.record().indexOf("id")).toInt();
                    newRecord.title = query.value(query.record().indexOf("title")).toString();
                    newRecord.username = query.value(query.record().indexOf("username")).toString();
                    newRecord.note = query.value(query.record().indexOf("note")).toString();

                    modify.push_back(newRecord);
                }
            }
        }
    } catch (...) {
        // exception
    }
}
