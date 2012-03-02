/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -p audiosource.h:audiosource.cpp -i types.h ../xml/audiosource.xml
 *
 * qdbusxml2cpp is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef AUDIOSOURCE_H_1330710471
#define AUDIOSOURCE_H_1330710471

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
 * Proxy class for interface org.bluez.AudioSource
 */
class OrgBluezAudioSourceInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.bluez.AudioSource"; }

public:
    OrgBluezAudioSourceInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~OrgBluezAudioSourceInterface();

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

Q_SIGNALS: // SIGNALS
    void PropertyChanged(const QString &in0, const QDBusVariant &in1);
};

namespace org {
  namespace bluez {
    typedef ::OrgBluezAudioSourceInterface AudioSource;
  }
}
#endif
