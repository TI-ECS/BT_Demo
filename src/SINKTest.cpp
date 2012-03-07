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

static QString BLUEZ_ALREADY_CONNECTED = "org.bluez.Error.AlreadyConnected";

SINKTest::SINKTest(QWidget *parent)
    :QWidget(parent),
    m_alsaSink(0)
{
    setupUi(this);

    m_pactl.closeReadChannel(QProcess::StandardOutput);
    m_pactl.closeWriteChannel();

    m_pulse.closeReadChannel(QProcess::StandardOutput);
    m_pulse.closeWriteChannel();
}

SINKTest::~SINKTest()
{
    shutdown();
}

void SINKTest::initLoadLoopbackResult(int exitCode, QProcess::ExitStatus)
{
    disconnect(&m_pactl, SIGNAL(finished(int, QProcess::ExitStatus)), this,
               SLOT(initLoadLoopbackResult(int, QProcess::ExitStatus)));

    if (exitCode != 0) {
        emit deviceReady(false);
        return;
    }

    emit deviceReady(true);
}

void SINKTest::initLoadLoopback()
{
    QStringList args;

    args << "load-module" << "module-loopback"
         << QString("source=bluez_source.%1").arg(m_sourceAddr)
         << "sink=audioout";

    m_pactl.start("/usr/bin/pactl", args);
    if (!m_pactl.waitForStarted(10000))
        close();

    connect(&m_pactl, SIGNAL(finished(int, QProcess::ExitStatus)), this,
                SLOT(initLoadLoopbackResult(int, QProcess::ExitStatus)));
}

void SINKTest::initConnectRemoteResult(QDBusPendingCallWatcher *watcher)
{
    watcher->deleteLater();

    QDBusPendingReply<> reply = *watcher;
    if (!reply.isValid()) {
        emit deviceReady(false);
        return;
    }

    initLoadLoopback();
}

void SINKTest::initConnectRemote()
{
    QDBusPendingCallWatcher *watcher =
                new QDBusPendingCallWatcher(m_audioSource->Connect(), this);

    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(initConnectRemoteResult(QDBusPendingCallWatcher*)));
}

void SINKTest::initStartPulse()
{
    QStringList args;

    args << "-n" << "-D" << "--realtime" << "--disallow-exit"
         << "--high-priority" << "--exit-idle-time=-1"
         << "--resample-method=trivial" << "--fail"
         << "-L" << "module-native-protocol-unix"
         << "-L" << "module-bluetooth-discover"
         << "-L" << QString("module-alsa-sink device=\"hw:%1,0\" sink_name=audioout").arg(m_alsaSink);

    m_pulse.start("/usr/bin/pulseaudio", args);
    if (!m_pulse.waitForStarted(10000))
        close();

    initConnectRemote();
}

void SINKTest::initTest(DeviceItem *device)
{
    m_audioSource = new AudioSource(BLUEZ_SERVICE_NAME,
                                    device->device()->path(),
                                    QDBusConnection::systemBus());

    m_sourceAddr = device->address().replace(':', '_');

    // Finish previous instance of pulseaudio
    shutdownPulse();

    initStartPulse();
}

void SINKTest::shutdownPulse()
{
    QProcess k;

    k.closeWriteChannel();
    k.start("/usr/bin/pulseaudio", QStringList() << "--kill");
    if (!k.waitForFinished(10000))
        close();
}

void SINKTest::shutdown()
{
    shutdownPulse();

    delete m_audioSource;
    m_audioSource = NULL;
}

void SINKTest::done()
{
    m_audioSource->Disconnect();
    shutdown();

    emit testFinished();
}
