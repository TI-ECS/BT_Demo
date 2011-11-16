/*
* Bluetooth Demo
*
* Bluetooth demo application.
*
* Copyright (C) 2011 ProFUSION embedded systems
*
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of ProFUSION embedded systems nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#ifndef _TYPES_H_
#define _TYPES_H_

#define REGISTER_TYPES()                                        \
    do {                                                        \
        qRegisterMetaType<ServiceMap>("ServiceMap");            \
        qDBusRegisterMetaType<ServiceMap>();                    \
        qRegisterMetaType<QStringMap>("QStringMap");            \
        qDBusRegisterMetaType<QStringMap>();                    \
        qRegisterMetaType<QVariantMapList>("QVariantMapList");  \
        qDBusRegisterMetaType<QVariantMapList>();               \
        qRegisterMetaType<QStringMapList>("QStringMapList");    \
        qDBusRegisterMetaType<QStringMapList>();                \
        qRegisterMetaType<QVariantMapList>("QVariantMapList");  \
        qDBusRegisterMetaType<QVariantMapList>();               \
    } while (0);

static QString BLUEZ_SERVICE_NAME("org.bluez");
static QString BLUEZ_MANAGER_PATH("/");
static QString OPENOBEX_SERVICE_NAME("org.openobex");
static QString OPENOBEX_SERVICE_NAME_CLIENT("org.openobex.client");
static QString OPENOBEX_SERVICE_NAME_SERVER("org.openobex.Server");
static QString OPENOBEX_MANAGER_PATH("/");

typedef QMap<uint, QString> ServiceMap;
Q_DECLARE_METATYPE(ServiceMap)

typedef QMap<QString, QString> QStringMap;
Q_DECLARE_METATYPE(QStringMap)

typedef QList<QVariantMap> QVariantMapList;
Q_DECLARE_METATYPE(QVariantMapList)

typedef QList<QStringMap> QStringMapList;
Q_DECLARE_METATYPE(QStringMapList)

typedef QList<QVariantMapList> QVariantMapListList;
Q_DECLARE_METATYPE(QVariantMapListList)


#endif // _TYPES_H_
