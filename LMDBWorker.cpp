#include "LMDBWorker.h"
#include <lmdb.h>
#include <QDebug>

LMDBWorker::LMDBWorker() {
    // Inicializa la base de datos LMDB
    int ret = mdb_env_create(&env);
    if (ret != MDB_SUCCESS) {
        qWarning("Error al crear el entorno de LMDB.");
    }
    ret = mdb_env_open(env, "./lmdb_test", 0, 0664);
    if (ret != MDB_SUCCESS) {
        qWarning("Error al abrir el entorno de LMDB.");
    }

    // Iniciar transacción
    ret = mdb_txn_begin(env, nullptr, 0, &txn);
    if (ret != MDB_SUCCESS) {
        qWarning("Error al iniciar transacción.");
    }

    // Abrir base de datos
    ret = mdb_dbi_open(txn, nullptr, 0, &dbi);
    if (ret != MDB_SUCCESS) {
        qWarning("Error al abrir la base de datos.");
    }
}

LMDBWorker::~LMDBWorker() {
    // Confirmar transacción y cerrar recursos
    mdb_txn_commit(txn);
    mdb_dbi_close(env, dbi);
    mdb_env_close(env);
}

void LMDBWorker::saveUserData(const QString &username, const QString &password) {
    if (username.isEmpty() || password.isEmpty()) {
        qDebug() << "Username or password cannot be empty.";
        return;
    }

    MDB_val key, data;

    // Preparar los datos para almacenar
    key.mv_size = username.length();
    key.mv_data = (void*)username.toUtf8().constData();

    data.mv_size = password.length();
    data.mv_data = (void*)password.toUtf8().constData();

    // Guardar los datos en la base de datos
    int rc = mdb_put(txn, dbi, &key, &data, 0);
    if (rc != MDB_SUCCESS) {
        qDebug() << "Error saving user data:" << rc;
        mdb_txn_abort(txn);  // Abortamos la transacción en caso de error
        return;
    }

    // Confirmar la transacción
    rc = mdb_txn_commit(txn);
    if (rc != MDB_SUCCESS) {
        qDebug() << "Error committing transaction:" << rc;
        mdb_txn_abort(txn);  // Abortamos si hay un error
    }

    qDebug() << "Usuario guardado exitosamente.";
}

QString LMDBWorker::readData(const QString &key) {
    MDB_val mdbKey, mdbValue;
    mdbKey.mv_size = key.length();
    mdbKey.mv_data = const_cast<char *>(key.toStdString().c_str());

    int ret = mdb_get(txn, dbi, &mdbKey, &mdbValue);
    if (ret == MDB_SUCCESS) {
        return QString::fromUtf8(static_cast<char *>(mdbValue.mv_data), mdbValue.mv_size);
    } else {
        return QString();
    }
}

void LMDBWorker::writeData(const QString &key, const QString &value) {
    MDB_val mdbKey, mdbValue;
    mdbKey.mv_size = key.length();
    mdbKey.mv_data = const_cast<char *>(key.toStdString().c_str());

    mdbValue.mv_size = value.length();
    mdbValue.mv_data = const_cast<char *>(value.toStdString().c_str());

    int ret = mdb_put(txn, dbi, &mdbKey, &mdbValue, 0);
    if (ret != MDB_SUCCESS) {
        qDebug() << "Error writing data: " << ret;
    }
}

