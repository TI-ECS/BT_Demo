/*
 *    BlueZ demo
 *    Copyright (C) 2011  ProFUSION embedded systems
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
