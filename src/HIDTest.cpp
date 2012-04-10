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

#include "HIDTest.h"

#include <QWSServer>
#include <unistd.h>

#define MOUSE_VAR "QWS_MOUSE_PROTO"

using namespace org::bluez;

static QString HID_CONNECTED = "org.bluez.Error.AlreadyConnected";

static const QStringList getInputDevices()
{
    QDir dir;
    QStringList inputs;

    dir = QDir("/dev/input/");
    if (!dir.exists()) {
        qCritical() << "can't open /dev/input/";
        return QStringList();
    }

    foreach(QString input, dir.entryList(QDir::AllEntries | QDir::System)) {
        if (input.startsWith("event", Qt::CaseInsensitive))
            inputs << QString("USB:/dev/input/" + input);
    }

    return inputs;
}

HIDTest::HIDTest(QWidget *parent)
    :QWidget(parent)
{
    setupUi(this);
    defaultMouse = QString(qgetenv(MOUSE_VAR));

    hid = NULL;
}

HIDTest::~HIDTest()
{
    if (hid)
        delete hid;
}

void HIDTest::initTest(DeviceItem *device)
{
    isMouse = (device->icon() == "input-mouse") ? true : false;
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

    qputenv(MOUSE_VAR, defaultMouse.toAscii());

#ifndef DEBUG
    QWSServer * server = QWSServer::instance();
    if (server)
        server->openMouse();
#endif

    emit testFinished();
}

void HIDTest::connectResult(QDBusPendingCallWatcher *watcher)
{
    watcher->deleteLater();

    QDBusPendingReply<> reply = *watcher;
    if (!reply.isValid()) {
        if (reply.isError() && reply.error().name() != HID_CONNECTED) {
            delete hid;
            hid = NULL;
            emit deviceReady(false);
            return;
        }
    }

    if (isMouse) {
        sleep(2);
        QByteArray value = QByteArray(defaultMouse.toAscii());
        foreach(QString input, getInputDevices()) {
            value.append(" ");
            value.append(input.toAscii());
        }

        qputenv(MOUSE_VAR, value);
#ifndef DEBUG
        QWSServer * server = QWSServer::instance();
        if (server)
            server->openMouse();
#endif
    }

    emit deviceReady(true);
}
