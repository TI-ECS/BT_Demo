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

#ifndef _DEVICEITEM_H_
#define _DEVICEITEM_H_

#include "device.h"

#include <QMetaType>
#include <QString>

using namespace org::bluez;

class DeviceItem : public QObject
{
Q_OBJECT

public:
    DeviceItem(QString service, QString adapter_path, QString address,
               QString name, QString icon, bool paired, bool trusted,
               QObject *parent = 0);
    DeviceItem(QString service, QString adapter_path, QString address,
               QObject *parent = 0);
    DeviceItem(const DeviceItem &other, QObject *parent = 0);
    ~DeviceItem();


    QString address()              const {return m_address;}
    QString icon()                 const {return m_icon;}
    QString name()                 const {return m_name;}
    QStringList properties()       const {return m_properties;}
    bool paired()                  const {return m_paired;}
    bool trusted()                 const {return m_trusted;}
    org::bluez::Device* device()         {return m_device;}
    void unpair();
    void updateName(const QString &name);

signals:
    void valuesChanged(const DeviceItem *self);

private slots:
    void deviceProperties(QDBusPendingCallWatcher *watcher);
    void propertyChanged(const QString &property,
                         const QDBusVariant &variant);

private:
    QString m_address, m_icon, m_name, m_alias;
    QStringList m_properties;
    bool m_paired, m_trusted;
    org::bluez::Device *m_device;
};

Q_DECLARE_METATYPE(DeviceItem *)

#endif /* _DEVICEITEM_H_ */
