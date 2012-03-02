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

#include "DeviceItem.h"

#include <QString>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

struct service_name {
    QString uuid;
    QString name;
};

static service_name services[] = {
    { QString("00001101-0000-1000-8000-00805f9b34fb"), QString("SPP") },
    { QString("00001105-0000-1000-8000-00805f9b34fb"), QString("OPP") },
    { QString("00001106-0000-1000-8000-00805f9b34fb"), QString("FTP") },
    { QString("0000110b-0000-1000-8000-00805f9b34fb"),
      QString("A2DP Audio Sink") },
    { QString("00001124-0000-1000-8000-00805f9b34fb"),
      QString("Human Interface Device (HID)") }
};

static QString getServiceName(const QString &uuid)
{
    for (unsigned i = 0; i < ARRAY_SIZE(services); i++)
        if (services[i].uuid == uuid)
            return services[i].name;

    return QString();
}

DeviceItem::DeviceItem(QString service, QString adapter_path, QString address,
                       QString name, QString icon, bool paired, bool trusted,
                       QObject *parent)
    :QObject(parent)
{

    m_address = address;
    m_name = name;
    m_icon = icon;

    m_paired = paired;
    m_trusted = trusted;

    QString addr = address;
    QString devicePath = QString("%1/dev_%2").arg(adapter_path).
        arg(addr.replace(':', '_'));
    m_device = new Device(service, devicePath,
                          QDBusConnection::systemBus());

    connect(m_device, SIGNAL(PropertyChanged(const QString&,
                                             const QDBusVariant&)),
            this, SLOT(propertyChanged(const QString&,
                                       const QDBusVariant&)));
    QDBusPendingCallWatcher *watcher;
    watcher = new QDBusPendingCallWatcher(m_device->GetProperties(), this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(deviceProperties(QDBusPendingCallWatcher*)));
}

DeviceItem::DeviceItem(QString service, QString adapter_path, QString address,
                       QObject *parent)
    :QObject(parent)
{
    m_address = address;
    QString addr = address;
    QString devicePath = QString("%1/dev_%2").arg(adapter_path).
        arg(addr.replace(':', '_'));
    m_device = new Device(service, devicePath,
                          QDBusConnection::systemBus());

    connect(m_device, SIGNAL(PropertyChanged(const QString&,
                                             const QDBusVariant&)),
            this, SLOT(propertyChanged(const QString&,
                                       const QDBusVariant&)));
    QDBusPendingCallWatcher *watcher;
    watcher = new QDBusPendingCallWatcher(m_device->GetProperties(), this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(deviceProperties(QDBusPendingCallWatcher*)));
}

DeviceItem::DeviceItem(const DeviceItem &other, QObject *parent)
    :QObject(parent)
{
    m_name = other.m_name;
    m_icon = other.m_icon;
    m_address = other.m_address;

    m_paired = other.m_paired;
    m_trusted = other.m_trusted;
    m_device = other.m_device;
    if (m_device) {
        connect(m_device, SIGNAL(PropertyChanged(const QString&,
                                                 const QDBusVariant&)),
                this, SLOT(propertyChanged(const QString&,
                                           const QDBusVariant&)));
        QDBusPendingCallWatcher *watcher;
        watcher = new QDBusPendingCallWatcher(m_device->GetProperties(), this);
        connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                this, SLOT(deviceProperties(QDBusPendingCallWatcher*)));
    }
}

DeviceItem::~DeviceItem()
{
    disconnect(this, 0, 0, 0);
    if (m_device)
        delete m_device;
}

void DeviceItem::propertyChanged(const QString &property,
                                 const QDBusVariant &value)
{
    if (property == QString("Name")) {
        m_name = value.variant().toString();
        emit valuesChanged(this);
    } else if (property == QString("Paired")) {
        m_paired = value.variant().toBool();
        emit valuesChanged(this);
    } else if (property == QString("Trusted")) {
        m_trusted = value.variant().toBool();
        emit valuesChanged(this);
    } else if (property == QString("UUIDs")) {
        m_properties = QStringList();
        foreach (const QVariant &str, value.variant().toList()) {
            QString name = getServiceName(str.toString());
            if (!name.isEmpty())
                m_properties << name;
        }
        emit valuesChanged(this);
    }
}

void DeviceItem::deviceProperties(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<QVariantMap> reply = *watcher;
    if (!reply.isValid())
        return;

    QVariantMap properties = reply.value();
    m_properties = QStringList();
    foreach (const QVariant &str, properties.value("UUIDs").toList()) {
        QString name = getServiceName(str.toString());
        if (!name.isEmpty())
            m_properties << name;
    }

    m_paired = properties.value("Paired").toBool();
    m_trusted = properties.value("Trusted").toBool();
    m_address = properties.value("Address").toString();
    m_icon = properties.value("Icon").toString();
    m_name = properties.value("Name").toString();

    watcher->deleteLater();

    emit valuesChanged(this);
}

void DeviceItem::unpair()
{
    m_paired = false;
    emit valuesChanged(this);
}

void DeviceItem::updateName(const QString &name)
{
    m_name = name;
    emit valuesChanged(this);
}
