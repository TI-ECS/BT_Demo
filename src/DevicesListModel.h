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

#ifndef _DEVICESLISTMODEL_H_
#define _DEVICESLISTMODEL_H_

#include "DeviceItem.h"

#include <QAbstractListModel>

class DevicesListModel : public QAbstractListModel
{
Q_OBJECT

public:
    DevicesListModel(QObject *parent = 0);
    ~DevicesListModel();

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    void setDevicesList(QList<DeviceItem *> devices);

private slots:
    void deviceItemChanged(const DeviceItem *item);

private:
    QList<DeviceItem *> devicesList;
};

#endif /* _DEVICESLISTMODEL_H_ */
