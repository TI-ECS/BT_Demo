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

#ifndef _HIDTEST_H_
#define _HIDTEST_H_

#include "DeviceItem.h"
#include "input.h"
#include "ui_HIDTest.h"

class HIDTest : public QWidget, public Ui::HIDTest
{
Q_OBJECT

public:

    HIDTest(QWidget *parent = 0);
    ~HIDTest();

public slots:

    void initTest(DeviceItem *device);

private slots:

    void connectResult(QDBusPendingCallWatcher *watcher);
    void done();

signals:

    void deviceReady(bool);
    void testFinished();

private:

    QString defaultMouse;
    bool isMouse;
    org::bluez::Input *hid;
};

#endif /* _HIDTEST_H_ */
