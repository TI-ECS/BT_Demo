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

#include "HIDTest.h"

using namespace org::bluez;

static QString HID_CONNECTED = "org.bluez.Error.AlreadyConnected";

HIDTest::HIDTest(QWidget *parent)
    :QWidget(parent)
{
    setupUi(this);

    hid = NULL;
}

HIDTest::~HIDTest()
{
    if (hid)
        delete hid;
}

void HIDTest::initTest(DeviceItem *device)
{
    text_hid->clear();
    text_hid->setFocus(Qt::OtherFocusReason);
    hid = new Input(BLUEZ_SERVICE_NAME,
                    device->device()->path(),
                    QDBusConnection::systemBus());

    QDBusPendingCallWatcher *watcher;
    watcher = new QDBusPendingCallWatcher(hid->Connect(), this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(connectResult(QDBusPendingCallWatcher*)));
}

void HIDTest::done()
{
    // It's hack is necessary because a bluez bug.
    // hid->Disconnect();
    delete hid;
    hid = NULL;

    emit testFinished();
}

void HIDTest::connectResult(QDBusPendingCallWatcher *watcher)
{
    watcher->deleteLater();

    QDBusPendingReply<> reply = *watcher;
    if (!reply.isValid()) {
        if (reply.isError() && reply.error().name() == HID_CONNECTED) {
            emit deviceReady(true);
            return;
        }

        delete hid;
        hid = NULL;
        emit deviceReady(false);
    } else
        emit deviceReady(true);
}
