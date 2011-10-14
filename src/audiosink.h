/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -p audiosink.h:audiosink.cpp -i types.h ../xml/audiosink.xml
 *
 * qdbusxml2cpp is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef AUDIOSINK_H_1312840133
#define AUDIOSINK_H_1312840133

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
 * Proxy class for interface org.bluez.AudioSink
 */
class OrgBluezAudioSinkInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.bluez.AudioSink"; }

public:
    OrgBluezAudioSinkInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~OrgBluezAudioSinkInterface();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> Connect()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Connect"), argumentList);
    }

    inline QDBusPendingReply<> Disconnect()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Disconnect"), argumentList);
    }

    inline QDBusPendingReply<QVariantMap> GetProperties()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("GetProperties"), argumentList);
    }

    inline QDBusPendingReply<bool> IsConnected()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("IsConnected"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void Connected();
    void Disconnected();
    void Playing();
    void PropertyChanged(const QString &in0, const QDBusVariant &in1);
    void Stopped();
};

namespace org {
  namespace bluez {
    typedef ::OrgBluezAudioSinkInterface AudioSink;
  }
}
#endif
