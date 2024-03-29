/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -p adapter.h:adapter.cpp -i types.h ../xml/adapter.xml
 *
 * qdbusxml2cpp is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef ADAPTER_H_1312840075
#define ADAPTER_H_1312840075

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
 * Proxy class for interface org.bluez.Adapter
 */
class OrgBluezAdapterInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.bluez.Adapter"; }

public:
    OrgBluezAdapterInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~OrgBluezAdapterInterface();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> CancelDeviceCreation(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(in0);
        return asyncCallWithArgumentList(QLatin1String("CancelDeviceCreation"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> CreateDevice(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(in0);
        return asyncCallWithArgumentList(QLatin1String("CreateDevice"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> CreatePairedDevice(const QString &in0, const QDBusObjectPath &in1, const QString &in2)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(in0) << qVariantFromValue(in1) << qVariantFromValue(in2);
        return asyncCallWithArgumentList(QLatin1String("CreatePairedDevice"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> FindDevice(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(in0);
        return asyncCallWithArgumentList(QLatin1String("FindDevice"), argumentList);
    }

    inline QDBusPendingReply<QVariantMap> GetProperties()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("GetProperties"), argumentList);
    }

    inline QDBusPendingReply<QList<QDBusObjectPath> > ListDevices()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("ListDevices"), argumentList);
    }

    inline QDBusPendingReply<> RegisterAgent(const QDBusObjectPath &in0, const QString &in1)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(in0) << qVariantFromValue(in1);
        return asyncCallWithArgumentList(QLatin1String("RegisterAgent"), argumentList);
    }

    inline QDBusPendingReply<> ReleaseSession()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("ReleaseSession"), argumentList);
    }

    inline QDBusPendingReply<> RemoveDevice(const QDBusObjectPath &in0)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(in0);
        return asyncCallWithArgumentList(QLatin1String("RemoveDevice"), argumentList);
    }

    inline QDBusPendingReply<> RequestSession()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("RequestSession"), argumentList);
    }

    inline QDBusPendingReply<> SetProperty(const QString &in0, const QDBusVariant &in1)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(in0) << qVariantFromValue(in1);
        return asyncCallWithArgumentList(QLatin1String("SetProperty"), argumentList);
    }

    inline QDBusPendingReply<> StartDiscovery()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("StartDiscovery"), argumentList);
    }

    inline QDBusPendingReply<> StopDiscovery()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("StopDiscovery"), argumentList);
    }

    inline QDBusPendingReply<> UnregisterAgent(const QDBusObjectPath &in0)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(in0);
        return asyncCallWithArgumentList(QLatin1String("UnregisterAgent"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void DeviceCreated(const QDBusObjectPath &in0);
    void DeviceDisappeared(const QString &in0);
    void DeviceFound(const QString &in0, const QVariantMap &in1);
    void DeviceRemoved(const QDBusObjectPath &in0);
    void PropertyChanged(const QString &in0, const QDBusVariant &in1);
};

namespace org {
  namespace bluez {
    typedef ::OrgBluezAdapterInterface Adapter;
  }
}
#endif
