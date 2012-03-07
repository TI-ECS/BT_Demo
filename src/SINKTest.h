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

#ifndef _SINKTEST_H_
#define _SINKTEST_H_

#include "DeviceItem.h"
#include "audiosource.h"
#include "ui_SINKTest.h"

#include <QWidget>

class SINKTest : public QWidget, public Ui::SINKTest
{
Q_OBJECT

public:

    SINKTest(QWidget *parent = 0);
    ~SINKTest();

public slots:

    void initTest(DeviceItem *device);

private slots:

    void done();

    void shutdownPulse();
    void shutdown();

    void initLoadLoopbackResult(int, QProcess::ExitStatus);
    void initLoadLoopback();
    void initConnectRemoteResult(QDBusPendingCallWatcher *watcher);
    void initConnectRemote();
    void initStartPulse();

    void propertyChanged(const QString&, const QDBusVariant&);
signals:

    void deviceReady(bool);
    void testFinished();

private:

    bool m_connected;
    unsigned m_alsaSink;
    QProcess m_pulse;
    QProcess m_pactl;
    QString m_sourceAddr;
    org::bluez::AudioSource *m_audioSource;
};

#endif /* _SINKTEST_H_ */
