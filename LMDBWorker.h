#ifndef LMDBWORKER_H
#define LMDBWORKER_H

#include <lmdb.h>
#include <QString>

class LMDBWorker {
public:
    LMDBWorker();
    ~LMDBWorker();

    QString readData(const QString &key);
    void writeData(const QString &key, const QString &value);
    void saveUserData(const QString &username, const QString &password);


private:
    MDB_env *env;
    MDB_dbi dbi;
    MDB_txn *txn;
};

#endif // LMDBWORKER_H
