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
