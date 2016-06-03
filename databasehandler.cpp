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

#include <cryptopp/eax.h>
#include <cryptopp/sha.h>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/base64.h>
#include <cryptopp/hex.h>

#include <QDebug>
#include <QSqlError>
//#include <QApplication>

#include <ctime>
#include <cstring>

#include "common.h"

using namespace std;
using namespace CryptoPP;

dataBaseHandler::dataBaseHandler() :
    loginFlag(false),
    storedKey(nullptr)
{
    _db = QSqlDatabase::addDatabase(kkCommon::dbType);
    _db.setDatabaseName(kkCommon::dbPath);
}

dataBaseHandler::~dataBaseHandler()
{
    expireKey();
}

void dataBaseHandler::expireKey()
{
    if (storedKey == nullptr) {
        return;
    }

    delete [] storedKey;
    storedKey = nullptr;

    loginFlag = false;
}

void dataBaseHandler::passwordToKey(const QString &password)
{
    expireKey();
    string pwd = password.toStdString();
    storedKey = new SecByteBlock(CryptoPP::AES::MAX_KEYLENGTH);
    for (size_t i = 0; i < CryptoPP::AES::MAX_KEYLENGTH; ++i) {
        if (i < pwd.length()) {
            storedKey->data()[i] = (byte)(pwd[i]);
        } else {
            storedKey->data()[i] = 0x00;
        }
    }
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

    return hashedPassword;
}

string dataBaseHandler::decrypt(const string &ciphertext)
{
    string cipher = decodeFromHex(ciphertext);
    ECB_Mode< AES >::Decryption d;
    d.SetKey(*storedKey, storedKey->size());
    StreamTransformationFilter decryptor(d, NULL);

    decryptor.Put((byte*)cipher.data(), cipher.size());
    decryptor.MessageEnd();

    size_t readysize = decryptor.MaxRetrievable();

    string plain(readysize, 0x00);
    decryptor.Get((byte*)plain.data(), plain.size());

    return plain;
}

string dataBaseHandler::encrypt(const string &plain)
{
    ECB_Mode< AES >::Encryption e;
    e.SetKey(*storedKey, storedKey->size());
    StreamTransformationFilter encryptor(e, NULL);

    encryptor.Put((byte*)plain.data(), plain.size());
    encryptor.MessageEnd();

    size_t readysize = encryptor.MaxRetrievable();

    string ciphertext(readysize, 0x00);
    encryptor.Get((byte*)ciphertext.data(), ciphertext.size());

    return encodeToHex(ciphertext);
}

string dataBaseHandler::encodeToHex(const std::string input)
{
    string encoded;

    HexEncoder encoder;
    encoder.Put((byte*)input.data(), input.size());
    encoder.MessageEnd();

    word64 size = encoder.MaxRetrievable();
    if(size)
    {
        encoded.resize(size);
        encoder.Get((byte*)encoded.data(), encoded.size());
    }

    return encoded;
}

string dataBaseHandler::decodeFromHex(const std::string input)
{
    string decoded;

    HexDecoder decoder;

    decoder.Put((byte*)input.data(), input.size());
    decoder.MessageEnd();

    word64 size = decoder.MaxRetrievable();
    if(size && size <= SIZE_MAX)
    {
        decoded.resize(size);
        decoder.Get((byte*)decoded.data(), decoded.size());
    }

    return decoded;
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
    if (!(needToCreateDatabase()) || (password.length() == 0) || (password.length() > 32))  {
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

    try
    {
        loginFlag = _db.open();
        expireKey();
        passwordToKey(password);
        // fetch user salt
        QSqlQuery query;
        query.clear();
        query.prepare("select salt from validation");
        loginFlag = loginFlag && query.exec() && (query.first());
        query.first();
        QString salt = query.value(query.record().indexOf("salt")).toString();
        // hash with salt
        QString hashedValue = hashPassword(password, salt);
        // matching hash
        query.clear();
        query.prepare("select * from validation where hash = :hashedvalue");
        query.bindValue(":hashedvalue", hashedValue);
        loginFlag = loginFlag && query.exec() && (query.first());
    } catch (...) {
        expireKey();
        loginFlag = false;
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
                    newRecord.title = decrypt(query.value(query.record().indexOf("title")).toString().toStdString());
                    newRecord.username = decrypt(query.value(query.record().indexOf("username")).toString().toStdString());
                    newRecord.password = decrypt(query.value(query.record().indexOf("password")).toString().toStdString());
                    newRecord.note = decrypt(query.value(query.record().indexOf("note")).toString().toStdString());

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
                    newRecord.title = decrypt(query.value(query.record().indexOf("title")).toString().toStdString());
                    newRecord.username = decrypt(query.value(query.record().indexOf("username")).toString().toStdString());
                    newRecord.password = decrypt(query.value(query.record().indexOf("password")).toString().toStdString());
                    newRecord.note = decrypt(query.value(query.record().indexOf("note")).toString().toStdString());

                    modify.push_back(newRecord);
                }
            }
        }
    } catch (...) {
        // exception
    }
}

void dataBaseHandler::saveOnIndex(const dataBaseHandler::modelData &data)
{
    if (!isReady()) {
        return;
    }
    QSqlQuery query;
    try {
        query.clear();
        query.prepare("update userData set title=:title, username=:username, password=:password note=:note where id=:id");
        query.bindValue(":title", QString::fromStdString(encrypt(data.title)));
        query.bindValue(":username", QString::fromStdString(encrypt(data.username)));
        query.bindValue(":password", QString::fromStdString(encrypt(data.password)));
        query.bindValue(":note", QString::fromStdString(encrypt(data.note)));
        query.bindValue(":id", data.index);
        if (!query.exec()) {
            qDebug() << query.lastError();
        }
    } catch (...) {
        // exception

    }
}

void dataBaseHandler::insertNew(const dataBaseHandler::modelData &data)
{
    if (!isReady()) {
        return;
    }
    QSqlQuery query;
    try {
        query.clear();
        query.prepare("insert into userData (title, username, password, note) values (:title,:username,:password,:note)");
        query.bindValue(":title", QString::fromStdString(encrypt(data.title)));
        query.bindValue(":username", QString::fromStdString(encrypt(data.username)));
        query.bindValue(":password", QString::fromStdString(encrypt(data.password)));
        query.bindValue(":note", QString::fromStdString(encrypt(data.note)));
        if (!query.exec()) {
            qDebug() << query.lastError();
        }
    } catch (...) {
        // exception
    }
}
