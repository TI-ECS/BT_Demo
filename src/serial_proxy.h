/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -p serial_proxy.h:serial_proxy.cpp -i types.h ../xml/serial_proxy.xml
 *
 * qdbusxml2cpp is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef SERIAL_PROXY_H_1313530261
#define SERIAL_PROXY_H_1313530261

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include "types.h"

/*
 * Proxy class for interface org.bluez.SerialProxy
 */
class OrgBluezSerialProxyInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.bluez.SerialProxy"; }

public:
    OrgBluezSerialProxyInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~OrgBluezSerialProxyInterface();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> Disable()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Disable"), argumentList);
    }

    inline QDBusPendingReply<> Enable()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Enable"), argumentList);
    }

    inline QDBusPendingReply<QVariantMap> GetInfo()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("GetInfo"), argumentList);
    }

    inline QDBusPendingReply<> SetSerialParameters(const QString &in0, uchar in1, uchar in2, const QString &in3)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(in0) << qVariantFromValue(in1) << qVariantFromValue(in2) << qVariantFromValue(in3);
        return asyncCallWithArgumentList(QLatin1String("SetSerialParameters"), argumentList);
    }

Q_SIGNALS: // SIGNALS
};

namespace org {
  namespace bluez {
    typedef ::OrgBluezSerialProxyInterface SerialProxy;
  }
}
#endif
