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
