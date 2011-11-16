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
#include "DevicesListModel.h"

#include <QImage>

DevicesListModel::DevicesListModel(QObject *parent)
    :QAbstractListModel(parent),
     devicesList()
{
}

DevicesListModel::~DevicesListModel()
{
}

int DevicesListModel::rowCount(const QModelIndex &) const
{
    return devicesList.count();
}

QVariant DevicesListModel::data(const QModelIndex &index,
                                int role) const
{
    if (!index.isValid())
        return QVariant();

    int devicesListSize = devicesList.size();
    int row = index.row();
    if ((row >= devicesListSize) || (row < 0))
        return QVariant();

    if (role == Qt::DisplayRole) {
        DeviceItem *it =  devicesList.at(index.row());

        QString properties;
        foreach (QString aux, it->properties())
            properties.append(QString("%1 ").arg(aux));

        QStringList list;
        list << it->name() << it->name() << it->address() << properties;
        return QVariant(list);
    } else if (role == Qt::UserRole) {
        return qVariantFromValue<DeviceItem*>(devicesList.at(index.row()));
    } else if (role == Qt::DecorationRole) {
        DeviceItem *it =  devicesList.at(index.row());
        return QImage(QString(":/images/" + it->icon() + ".png"));
    } else if (role == Qt::UserRole + 1) {
        DeviceItem *it = devicesList.at(index.row());
        if (it->paired())
            return QImage(":/images/device-paired.png");
        else
            return QImage(":/images/device-unpaired.png");
    } else if (role == Qt::UserRole + 2) {
        DeviceItem *it =  devicesList.at(index.row());
        if (it->trusted())
            return QImage(":/images/lock.png");
        else
            return QImage();
    } else
        return QVariant();
}

void DevicesListModel::setDevicesList(const QList<DeviceItem *> devices)
{
    devicesList = devices;
    foreach (DeviceItem *item, devices)
        connect(item, SIGNAL(valuesChanged(const DeviceItem*)), this,
                SLOT(deviceItemChanged(const DeviceItem*)));
    emit dataChanged(this->createIndex(0,0),
                     this->createIndex(devices.count(),0));
}

QVariant DevicesListModel::headerData(int, Qt::Orientation, int) const
{
    return QVariant();
}

void DevicesListModel::deviceItemChanged(const DeviceItem *item)
{
    int i = 0;
    foreach (DeviceItem *it, devicesList) {
        if (it == item) {
            emit dataChanged(this->createIndex(i,0),
                             this->createIndex(i,0));
            return;
        }
        i++;
    }
}
