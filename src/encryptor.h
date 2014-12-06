/*
 * encryptor.h - the header file of Encryptor class
 *
 * high-level API to encrypt/decrypt data that send to or receive from
 * another shadowsocks side.
 *
 * Copyright (C) 2014, Symeon Huang <hzwhuang@gmail.com>
 *
 * This file is part of the libQtShadowsocks.
 *
 * libQtShadowsocks is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * libQtShadowsocks is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with pdnsd; see the file COPYING. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QCryptographicHash>
#include "cipher.h"

namespace QSS {

class Encryptor : public QObject
{
    Q_OBJECT
public:
    explicit Encryptor(QObject *parent = 0);
    ~Encryptor();

    enum TYPE {TABLE, BOTAN};
    QByteArray decrypt(const QByteArray &);
    QByteArray encrypt(const QByteArray &);
    QByteArray decryptAll(const QByteArray &);//(de)encryptAll is for updreplay
    QByteArray encryptAll(const QByteArray &);
    void reset();

    static const QVector<quint8> octVec;
    static const QMap<QByteArray, QVector<int> > cipherMap;
    static QMap<QByteArray, QVector<int> > generateCihperMap();
    static int randomCompare(const quint8 &, const quint8 &, const quint32 &, const quint64 &);

    /*
     * Only need to call this function once if the encrpytion method and password don't change.
     * If you want to change the method and password, remember to call reset() function to remove
     * the old enCipher and(or) deCipher.
     * It's not recommended to change method and/or password on-process. The clean way to do that
     * is to delete and release the old library classes, then construct them with new values.
     */
    static void initialise(const QString &m, const QString &pwd);

private:
    static TYPE type;
    static QByteArray method;
    static QByteArray password;
    static QVector<quint8> encTable;
    static QVector<quint8> decTable;
    static int keyLen;
    static int ivLen;

    static void tableInit();
    static QVector<quint8> mergeSort(const QVector<quint8> &, quint32, quint64);
    static void evpBytesToKey();
    bool selfTest();

protected:
    Cipher *enCipher;
    Cipher *deCipher;
    static QByteArray key;
};

}

#endif // ENCRYPTOR_H
