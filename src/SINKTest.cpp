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

#include "SINKTest.h"
#include "types.h"


using namespace org::bluez;


SINKTest::SINKTest(QWidget *parent)
    :QWidget(parent)
{
    setupUi(this);
}

SINKTest::~SINKTest()
{
    if (audioSource)
        delete audioSource;
}

void SINKTest::initTest(DeviceItem *device)
{
    audioSource = new AudioSource(BLUEZ_SERVICE_NAME,
                                  device->device()->path(),
                                  QDBusConnection::systemBus());

    QDBusPendingCallWatcher *watcher;
    watcher = new QDBusPendingCallWatcher(audioSource->Connect(), this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(connectResult(QDBusPendingCallWatcher*)));
}

void SINKTest::done()
{
    audioSource->Disconnect();

    delete audioSource;
    audioSource = NULL;

    emit testFinished();
}

void SINKTest::connectResult(QDBusPendingCallWatcher *watcher)
{
    watcher->deleteLater();

    QDBusPendingReply<> reply = *watcher;
    if (!reply.isValid() && reply.isError()) {
        delete audioSource;
        audioSource = NULL;
        emit deviceReady(false);
        return;
    }

    emit deviceReady(true);
}
